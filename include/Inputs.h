#ifndef INPUTS_H
#define INPUTS_H

#include "Camera.h"
#include "Chunk.h"

class Inputs
{
protected:
	bool keyF10;
	bool verticeMode;
public:
	Inputs();
	void processInput(GLFWwindow* window, Camera& camera);
	bool getVerticeMode();
};

#endif