#include "Generation.h"

Generation::Generation()
{
	std::vector<std::vector<int>> worldData;
	for (int i = -3; i <= 3; i ++)
			{
		for (int j = -3; j <= 3; j++)
		{
			worldData.push_back({ i, j });
			Chunk* newChunk = new Chunk(i, j);
			chunks.push_back(newChunk);
		}
	}
}

void Generation::ChunkGeneration()
{
	for (auto& chunk : chunks)
	{
		chunk->Generation();
	}
}

void Generation::ChunkBind(Camera& camera, GLFWwindow* window, bool verticeMode)
{
	for (auto& chunk : chunks)
	{
		chunk->BindBloc(camera, window, verticeMode);
	}
}

void Generation::Delete()
{
	for (auto& chunk : chunks)
	{
		delete chunk;
	}
}