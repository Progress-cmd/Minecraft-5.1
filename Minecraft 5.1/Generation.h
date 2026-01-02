#ifndef GENERATION_H
#define GENERATION_H

#include <vector>
#include "Chunk.h"
#include <cmath>


class Generation
{
protected:
	std::vector<Chunk*> chunks;
	std::vector<std::vector<int>> worldData;

public:
	Generation();
	void ChunkGeneration();
	void ChunkBind(Camera& camera, GLFWwindow* window, bool verticeMode);
	Chunk* getChunk(int cx, int cz);
	uint8_t getBlock(int x, int y, int z);
	void Delete();
};

#endif