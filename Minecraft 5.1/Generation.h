#ifndef GENERATION_H
#define GENERATION_H

#include <vector>
#include "Chunk.h"


class Generation
{
protected:
	std::vector<Chunk*> chunks;
	std::vector<std::vector<int>> worldData;

public:
	Generation();
	void ChunkGeneration();
	void ChunkBind(Camera& camera, GLFWwindow* window, bool verticeMode);
	void Delete();
};

#endif