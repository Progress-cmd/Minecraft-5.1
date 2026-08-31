#include "Camera.h"

Camera::Camera(int width, int height, glm::vec3 position)
{
	Camera::width = width;
	Camera::height = height;
	Position = position;
}

void Camera::Matrix(float FOVdeg, float nearPlan, float farPlan, Shader& shader, const char* uniform)
{
	glm::mat4 view = glm::mat4(1.0f); // initialisation de la matrice de vue en tant qu'indentit�e
	glm::mat4 projection = glm::mat4(1.0f); // initialisation de la matrice de projection en tant qu'indentit�e

	view = glm::lookAt(Position, Position + Orientation, Up); // fct (position de la cam�ra, cible � regarder, vecteur vertical)
	projection = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlan, farPlan);  // d�termine le champs de vision de la cam�ra, en radian. Et la distance minimale et maximale � afficher

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
		speed = 1.0f;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
	{
		speed = 0.1f;
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		// GLFW_CURSOR_DISABLED capture la souris et g�re le mouvement en interne,
		// sans qu'on ait besoin de la replacer nous-m�me au centre � chaque frame.
		// (GLFW_CURSOR_HIDDEN + glfwSetCursorPos provoquait des sauts de rotation
		// extr�mes sous Linux/X11/Wayland : repositionner la souris pouvait g�n�rer
		// un faux �v�nement de mouvement, mesur� en plus du vrai d�placement.)
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		double mouseX;
		double mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY); // attribut les coordonn�es du curseur

		if (leftClic)
		{
			// premier clic : on m�morise la position sans calculer de rotation,
			// pour �viter un saut bas� sur un ancien d�placement non pertinent
			lastMouseX = mouseX;
			lastMouseY = mouseY;
			leftClic = false;
		}

		// rotation bas�e sur le d�placement depuis la derni�re frame, pas depuis le centre
		float rotX = sensitivity * (float)(mouseY - lastMouseY) / height;
		float rotY = sensitivity * (float)(mouseX - lastMouseX) / width;

		lastMouseX = mouseX;
		lastMouseY = mouseY;

		glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up))); // Calcule de mani�re pr�ventive la nouvelle orientation de la cam�ra

		if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f)) // emp�che la cam�ra de faire un tour verticale complet (un tonneau)
		{
			Orientation = newOrientation; // effectue la rotation en X
		}

		Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up); // effectue la rotation en Y
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // fait r�appara�tre le curseur
		leftClic = true;
	}

	if (glfwGetKey(window, GLFW_KEY_F11) == GLFW_PRESS)
	{
		if (!keyF11) // touche press�e pour la premi�re fois
		{
			keyF11 = true;

			if (pleinEcran)
			{
				// revenir en fen�tre
				glfwSetWindowMonitor(window, NULL, 100, 100, 800, 800, GLFW_DONT_CARE);
				glViewport(0, 0, 800, 800);
				width = 800;
				height = 800;
				pleinEcran = false;
			}
			else
			{
				// passer en plein �cran
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
		keyF11 = false; // la touche a �t� rel�ch�e
	}
}

glm::vec3 Camera::getPosition()
{
	return Position;
}