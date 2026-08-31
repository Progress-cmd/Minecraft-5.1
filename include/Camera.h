#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <gtx/rotate_vector.hpp>
#include <gtx/vector_angle.hpp>

#include "shaderClass.h"

class Camera
{
protected:
	glm::vec3 Position; // position de la cam�ra
	glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f); // orientation de la cam�ra
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f); // axe vertical de la cam�ra

	int width; // taille de la cam�ra (x)
	int height; // taille de la cam�ra (y)

	float speed = 0.5f; // vitesse de la cam�ra
	float sensitivity = 300.0f; // sensibilit� de la cam�ra

	bool leftClic = true;
	double lastMouseX = 0.0;
	double lastMouseY = 0.0;

	bool keyF11 = false;
	bool pleinEcran = false;

public:
	Camera(int width, int height, glm::vec3 position); // constucteur basique

	void Matrix(float FOVdeg, float nearPlan, float farPlan, Shader& shader, const char* uniform); // cr�ation et envoie des matrices de vues et projections aux shaders
	void Inputs(GLFWwindow* window); // g�re les entr�e
	glm::vec3 getPosition();
};

#endif 