#include "Inputs.h"
#include "Camera.h"

Inputs::Inputs()
{
	keyF10 = false;
	verticeMode = false;
}

void Inputs::processInput(GLFWwindow* window, Camera& camera)
{
	camera.Inputs(window);
		if (glfwGetKey(window, GLFW_KEY_F10) == GLFW_PRESS)
	{
		if (keyF10)
		{
			keyF10 = false;
		}
		verticeMode = true;
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else if (glfwGetKey(window, GLFW_KEY_F10) == GLFW_RELEASE)
	{
		verticeMode = false;
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		keyF10 = true;
	}
}

bool Inputs::getVerticeMode()
{
	return verticeMode;
}