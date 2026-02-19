#include "Generation.h"
#include "Camera.h"
#include "Texture.h"

Generation::Generation(int seed)
{
	m_sharedShader = new Shader("default.vert", "default.frag");
	m_sharedTexture = new Texture("bitmap.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	m_sharedNoise = new Noise(seed);

	start();

	// Mise à jour du monde
	updateWorld({ 0, 128, 0});

	// Transfert des données CPU -> GPU
	updateMainThread();
}

Generation::~Generation() {
	stop();
	Delete();
	if (m_sharedShader) { m_sharedShader->Delete(); delete m_sharedShader; }
	if (m_sharedTexture) { m_sharedTexture->Delete(); delete m_sharedTexture; }
	//if (m_sharedNoise) { m_sharedNoise->Delete(); delete m_sharedNoise; }
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

		// 1. On récupère la requête
		{
			std::lock_guard<std::mutex> lock(meshMutex);
			if (!meshRequests.empty()) {
				coords = meshRequests.front();
				meshRequests.pop();
				found = true;
			}
		}
			
		if (found) {
			Chunk* chunk = getChunk(coords.first, coords.second);
			if (chunk) {
				// ÉTAPE A : On prévient que le CPU travaille
				chunk->setBeingBuilt(true);

				// ÉTAPE B : Calcul lourd (buildMeshCPU)
				ChunkData data = chunk->buildMeshCPU();

				// ÉTAPE C : On a fini le calcul CPU
				chunk->setBeingBuilt(false);

				// ÉTAPE D : On envoie les données vers la file d'attente
				std::lock_guard<std::mutex> lock(meshMutex);
				readyMeshes.push(data);
			}
		}
		else {
			std::this_thread::sleep_for(std::chrono::milliseconds(2));
		}
	}
}

void Generation::updateMainThread() {
	// Cette fonction tourne sur le thread principal (OpenGL context)
	std::lock_guard<std::mutex> lock(meshMutex);

	// Limite à 2 uploads par frame pour garder le jeu fluide
	int uploadCount = 0;

	while (!readyMeshes.empty() && uploadCount < 2) { // <--- AJOUT DE LA LIMITE
		ChunkData data = readyMeshes.front();
		readyMeshes.pop();

		std::lock_guard<std::recursive_mutex> lockChunk(chunkMutex);
		Chunk* chunk = getChunk(data.cx, data.cz);
		if (chunk) {
			chunk->uploadMeshToGPU(data);
		}
		uploadCount++; // <--- INCREMENTATION
	}
}

void Generation::draw(Camera& camera, bool wireframeMode) {
	int playerChunkX = (int)floor(camera.getPosition().x / 16.0f);
	int playerChunkZ = (int)floor(camera.getPosition().z / 16.0f);

	// VERROUILLE la liste pendant le dessin
	std::lock_guard<std::recursive_mutex> lock(chunkMutex);

	for (auto const& [key, chunk] : chunkMap) {
		if (!chunk) continue;

		int dx = chunk->getX() - playerChunkX;
		int dz = chunk->getZ() - playerChunkZ;

		// On ne dessine que ce qui est dans la distance de rendu
		if (dx * dx + dz * dz <= RenderDistance * RenderDistance) {
			chunk->draw(camera, wireframeMode, RenderDistance);
		}
	}
}

void Generation::ChunkCreate(int cx, int cz) {
	if (getChunk(cx, cz)) return;

	Chunk* newChunk = new Chunk(cx, cz, this, m_sharedShader, m_sharedTexture, m_sharedNoise);

	{
		// VERROUILLE impérativement ici !
		std::lock_guard<std::recursive_mutex> lock(chunkMutex);
		chunkMap[{cx, cz}] = newChunk;
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
	auto it = chunkMap.find({ cx, cz });
	if (it != chunkMap.end()) {
		return it->second;
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
		for (auto it = chunkMap.begin(); it != chunkMap.end(); ) {
			Chunk* chunk = it->second;
			int dx = chunk->getX() - playerChunkX;
			int dz = chunk->getZ() - playerChunkZ;

			if (dx * dx + dz * dz > GenerationDistance * GenerationDistance) {
				// On vérifie que le thread CPU n'est pas en train de build ce chunk
				if (!chunk->isBeingBuilt()) {
					delete chunk;
					it = chunkMap.erase(it);
				}
				else {
					++it; // On attend la prochaine frame pour le supprimer
				}
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
	// 1. On arrête d'abord le thread de calcul pour qu'il ne touche plus à rien
	stop();

	// 2. On verrouille pour la forme (même si le thread est stoppé)
	std::lock_guard<std::recursive_mutex> lock(chunkMutex);

	// 3. On vide les files d'attente pour éviter que des données 
	// fantômes ne soient traitées au redémarrage
	{
		std::lock_guard<std::mutex> mLock(meshMutex);
		std::queue<std::pair<int, int>> emptyReq;
		std::swap(meshRequests, emptyReq);

		std::queue<ChunkData> emptyReady;
		std::swap(readyMeshes, emptyReady);
	}

	// 4. On supprime les objets Chunk proprement
	for (auto& pair : chunkMap) {
		if (pair.second != nullptr) {
			delete pair.second;
		}
	}
	chunkMap.clear();
}