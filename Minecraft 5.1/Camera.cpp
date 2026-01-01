#include "Camera.h"

Camera::Camera(int width, int height, glm::vec3 position)
{
	Camera::width = width;
	Camera::height = height;
	Position = position;
}

void Camera::Matrix(float FOVdeg, float nearPlan, float farPlan, Shader& shader, const char* uniform)
{
	glm::mat4 view = glm::mat4(1.0f); // initialisation de la matrice de vue en tant qu'indentitée
	glm::mat4 projection = glm::mat4(1.0f); // initialisation de la matrice de projection en tant qu'indentitée

	view = glm::lookAt(Position, Position + Orientation, Up); // fct (position de la caméra, cible à regarder, vecteur vertical)
	projection = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlan, farPlan);  // détermine le champs de vision de la caméra, en radian. Et la distance minimale et maximale à afficher

	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(projection * view)); // affecte une valeur aux uniform et les envoie au shader
}

void Camera::Inputs(GLFWwindow* window)
{
	// ZQSD
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		Position += speed * glm::normalize(glm::vec3(Orientation.x, 0.0f, Orientation.z));
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		Position += speed * -glm::normalize(glm::cross(Orientation, Up)); // vecteur normal du produit scalaire des deux vecteurs
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		Position += speed * -glm::normalize(glm::vec3(Orientation.x, 0.0f, Orientation.z));
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		Position += speed * glm::normalize(glm::cross(Orientation, Up));
	}

	//SpaceShiftSpeed
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		Position += speed * Up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		Position += speed * -Up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		speed = 0.1f;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
	{
		speed = 0.01f;
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN); // cache le curseur

		if (leftClic)
		{
			glfwSetCursorPos(window, (width / 2), (height / 2));
			leftClic = false;
		}

		// enregistre les coordonnées du curseur
		double mouseX;
		double mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY); // attribut les coordonnées du curseur

		// Normalise et déplace les coordonnées du curseur de manière à ce qu'elles commencent au milieu de l'écran puis les « transforme » en degrés 
		float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
		float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

		glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up))); // Calcule de manière préventive la nouvelle orientation de la caméra

		if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f)) // empèche la caméra de faire un tour verticale complet (un tonneau)
		{
			Orientation = newOrientation; // effectue la rotation en X
		}

		Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up); // effectue la rotation en Y

		glfwSetCursorPos(window, (width / 2), (height / 2)); // replacement de la souris au centre, pour ne pas atteindre le bord de l'écran
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // fait réapparaître le curseur
		leftClic = true;
	}

	if (glfwGetKey(window, GLFW_KEY_F11) == GLFW_PRESS)
	{
		if (!keyF11) // touche pressée pour la première fois
		{
			keyF11 = true;

			if (pleinEcran)
			{
				// revenir en fenêtre
				glfwSetWindowMonitor(window, NULL, 100, 100, 800, 800, GLFW_DONT_CARE);
				glViewport(0, 0, 800, 800);
				width = 800;
				height = 800;
				pleinEcran = false;
			}
			else
			{
				// passer en plein écran
				GLFWmonitor* monitor = glfwGetPrimaryMonitor();
				const GLFWvidmode* mode = glfwGetVideoMode(monitor);
				glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
				glViewport(0, 0, mode->width, mode->height);
				width = mode->width;
				height = mode->height;
				pleinEcran = true;
			}
		}
	}
	else
	{
		keyF11 = false; // la touche a été relâchée
	}
}

glm::vec3 Camera::getPosition()
{
	return Position;
}