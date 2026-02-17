#include "Generation.h"
#include "Camera.h"
#include "Texture.h"

Generation::Generation()
{
	m_sharedShader = new Shader("default.vert", "default.frag");
	m_sharedTexture = new Texture("bitmap.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
}

Generation::~Generation() {
	stop();
	Delete();
	if (m_sharedShader) { m_sharedShader->Delete(); delete m_sharedShader; }
	if (m_sharedTexture) { m_sharedTexture->Delete(); delete m_sharedTexture; }
}

void Generation::start() {
	if (running) return;
	running = true;
	worker = std::thread(&Generation::workerLoop, this);
}

void Generation::stop() {
	running = false;
	if (worker.joinable()) worker.join();
}

void Generation::workerLoop() {
	while (running) {
		std::pair<int, int> coords;
		bool found = false;

		{
			std::lock_guard<std::mutex> lock(meshMutex);
			if (!meshRequests.empty()) {
				coords = meshRequests.front();
				meshRequests.pop();
				found = true;
			}
		}

		if (found) {
			std::lock_guard<std::recursive_mutex> lock(chunkMutex);
			Chunk* chunk = getChunk(coords.first, coords.second);

			if (chunk != nullptr) {
				// On appelle la fonction lourde sur le thread secondaire
				ChunkData data = chunk->buildMeshCPU();

				std::lock_guard<std::mutex> lock(meshMutex);
				readyMeshes.push(data);
			}
		}
		else {
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
	}
}

void Generation::updateMainThread() {
	// Cette fonction tourne sur le thread principal (OpenGL context)
	std::lock_guard<std::mutex> lock(meshMutex);
	while (!readyMeshes.empty()) {
		ChunkData data = readyMeshes.front();
		readyMeshes.pop();

		// On lock le chunkMutex pour être sûr que personne ne delete le chunk
		// pendant qu'on fait le uploadMeshToGPU
		std::lock_guard<std::recursive_mutex> lockChunk(chunkMutex);
		Chunk* chunk = getChunk(data.cx, data.cz);
		if (chunk) {
			chunk->uploadMeshToGPU(data);
		}
	}
}

void Generation::draw(Camera& camera, bool wireframeMode) {
	int playerChunkX = floor(camera.getPosition().x / 16.0f);
	int playerChunkZ = floor(camera.getPosition().z / 16.0f);

	// VERROUILLE la liste pendant le dessin
	std::lock_guard<std::recursive_mutex> lock(chunkMutex);

	for (auto& chunk : chunks) {
		if (!chunk) continue; // Sécurité supplémentaire

		int dx = chunk->getX() - playerChunkX;
		int dz = chunk->getZ() - playerChunkZ;

		// On ne dessine que ce qui est dans la distance de rendu
		if (dx * dx + dz * dz <= RenderDistance * RenderDistance) {
			chunk->draw(camera, wireframeMode);
		}
	}
}

void Generation::ChunkCreate(int cx, int cz) {
	if (getChunk(cx, cz)) return;

	Chunk* newChunk = new Chunk(cx, cz, this, m_sharedShader, m_sharedTexture);

	{
		// VERROUILLE impérativement ici !
		std::lock_guard<std::recursive_mutex> lock(chunkMutex);
		chunks.push_back(newChunk);
	}

	// Marquer les voisins comme "sales" pour boucher les trous
	Chunk* neighbors[4] = { getChunk(cx - 1, cz), getChunk(cx + 1, cz), getChunk(cx, cz - 1), getChunk(cx, cz + 1) };
	for (auto n : neighbors) if (n) n->markDirty();

	// Demander la génération du mesh
	std::lock_guard<std::mutex> lock(meshMutex);
	meshRequests.push({ cx, cz });
}

Chunk* Generation::getChunk(int cx, int cz)
{
	std::lock_guard<std::recursive_mutex> lock(chunkMutex); // On verrouille l'accès à la liste
	for (auto& chunk : chunks) {
		if (chunk->getX() == cx && chunk->getZ() == cz)
			return chunk;
	}
	return nullptr;
}

uint8_t Generation::getBlock(int x, int y, int z)
{
	if (y < 0 || y >= 128) return 0;

	int cx = static_cast<int>(std::floor(x / 16.0f));
	int cz = static_cast<int>(std::floor(z / 16.0f));

	Chunk* chunk = getChunk(cx, cz);
	if (!chunk) return 0;

	int lx = x - cx * 16;
	int lz = z - cz * 16;

	return chunk->getBlock(lx, y, lz);
}

void Generation::updateWorld(glm::vec3 playerPos) {
	int playerChunkX = static_cast<int>(std::floor(playerPos.x / 16.0f));
	int playerChunkZ = static_cast<int>(std::floor(playerPos.z / 16.0f));
	
	// 1. Suppression des chunks lointains
	{
		std::lock_guard<std::recursive_mutex> lock(chunkMutex);
		for (auto it = chunks.begin(); it != chunks.end(); ) {
			int dx = (*it)->getX() - playerChunkX;
			int dz = (*it)->getZ() - playerChunkZ;

			if (dx * dx + dz * dz > GenerationDistance * GenerationDistance) {
				delete* it; // Le destructeur de Chunk nettoie OpenGL
				it = chunks.erase(it);
			}
			else {
				++it;
			}
		}
	}

	// 2. Création des nouveaux chunks
	for (int x = -GenerationDistance; x <= GenerationDistance; x++) {
		for (int z = -GenerationDistance; z <= GenerationDistance; z++) {
			if (x * x + z * z <= GenerationDistance * GenerationDistance) {
				ChunkCreate(playerChunkX + x, playerChunkZ + z);
			}
		}
	}
}

void Generation::Delete() {
	// On verrouille pour éviter qu'un thread ne l'utilise pendant qu'on vide tout
	std::lock_guard<std::recursive_mutex> lock(chunkMutex);

	for (auto chunk : chunks) {
		if (chunk != nullptr) {
			delete chunk;
		}
	}
	chunks.clear();
}