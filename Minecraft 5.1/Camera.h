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
	glm::vec3 Position; // position de la caméra
	glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f); // orientation de la caméra
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f); // axe vertical de la caméra

	int width; // taille de la caméra (x)
	int height; // taille de la caméra (y)

	float speed = 0.1f; // vitesse de la caméra
	float sensitivity = 100.0f; // sensibilité de la caméra

	bool leftClic = true;

	bool keyF11 = false;
	bool pleinEcran = false;

public:
	Camera(int width, int height, glm::vec3 position); // constucteur basique

	void Matrix(float FOVdeg, float nearPlan, float farPlan, Shader& shader, const char* uniform); // création et envoie des matrices de vues et projections aux shaders
	void Inputs(GLFWwindow* window); // gère les entrée
	glm::vec3 getPosition();
};

#endif 