#include "Generation.h"

Generation::Generation()
{
	std::vector<std::vector<int>> worldData;
	for (int i = -5; i <= 5; i ++)
			{
		for (int j = -5; j <= 5; j++)
		{
			worldData.push_back({ i, j });
			Chunk* newChunk = new Chunk(i, j, this);
			chunks.push_back(newChunk);

			Chunk* left = getChunk(i - 1, j);
			Chunk* right = getChunk(i + 1, j);
			Chunk* down = getChunk(i, j - 1);
			Chunk* up = getChunk(i, j + 1);

			if (left)  left->markDirty();
			if (right) right->markDirty();
			if (down)  down->markDirty();
			if (up)    up->markDirty();
		}
	}
}

void Generation::ChunkGeneration()
{
	for (auto& chunk : chunks)
	{
		chunk->generateChunk();
	}
}

void Generation::ChunkBind(Camera& camera, GLFWwindow* window, bool verticeMode)
{
	for (auto& chunk : chunks)
	{
		chunk->BindBloc(camera, window, verticeMode);
	}
}

Chunk* Generation::getChunk(int cx, int cz)
{
	for (auto& chunk : chunks)
	{
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
	

void Generation::Delete()
{
	for (auto& chunk : chunks)
	{
		delete chunk;
	}
}