// ======================================== //
// ((((((((((((( LES INCLUDES ))))))))))))) //
// ======================================== //


// ============ Les librairie ============= //
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h> // Création et gestion de la fenêtre
#include <glm.hpp>
#include <stb_image.h>


// ========= Les fichiers sources ========= //
#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"


// ======================================== //
// (((((((((((( LE CODE SOURCE )))))))))))) //
// ======================================== //

using namespace std;

// ============ Les variables ============= //
bool pleinEcran = false;
bool keyF11 = false;

GLfloat vertices[] =
{
	-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower left corner
	0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower right corner
	0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f, // Upper corner
	-0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Inner left
	0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Inner right
	0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f // Inner down
};

GLuint indices[] =
{
	0, 3, 5, // Lower left triangle
	3, 2, 4, // Lower right triangle
	5, 4, 1 // Upper triangle
};

// ============ Les fonctions ============= //
void Erreur(int value)
{
	switch (value)
	{
	case 0:
		cout << "ERREUR 0: GLFW n'a pas pu s'initialiser." << endl; break;
	case 1:
		cout << "ERREUR 1: La fenêtre n'a pas pu être créée." << endl; break;
	case 2:
		cout << "ERREUR 2: Glad n'a pas pu se charger" << endl; break;
	}
}

void GLFWInit()
{
	glfwInit(); // Initialisation de GLFW

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // donne la version d'OpenGL
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6); // c'est 4.6 sa version
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //la version core contient toute les fonctions modernes (!= compatibility)
}

void escape(GLFWwindow* window, int key, int action)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE); // fermeture de la fenêtre
}

void f11(GLFWwindow* window, int key, int action)
{
	if (key == GLFW_KEY_F11 && action == GLFW_PRESS)
	{
		if (!keyF11)
			keyF11 = true;
		{
			if (pleinEcran)
			{
				glfwSetWindowMonitor(window, NULL, 100, 100, 800, 600, GLFW_DONT_CARE);
				glViewport(0, 0, 800, 600);
				pleinEcran = false;
			}
			else
			{
				GLFWmonitor* monitor = glfwGetPrimaryMonitor(); // récupère le moniteur principal
				const GLFWvidmode* mode = glfwGetVideoMode(monitor); // résolution du moniteur
				glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate); // met en plein écran
				glViewport(0, 0, mode->width, mode->height);
				pleinEcran = true;
			}
		}
	}
	else
		keyF11 = false;
}

void sortieClavier(GLFWwindow* window, int key, int scancode, int action, int mode) {
	escape(window, key, action);
	f11(window, key, action);
}



// =========== La fonction main =========== //
int main() {
	GLFWInit();
	if (!glfwInit()) { Erreur(0); return -1; } // vérifi que la librairie s'est bien initialisé

	GLFWwindow* window = glfwCreateWindow(800, 800, "Minecraft", NULL, NULL); // création de la fenêtre
	if (window == NULL) { Erreur(1); glfwTerminate(); return -1; } // vérifi que la window est bien créé
	glfwMakeContextCurrent(window); // dit à glfw d'utiliser la fenêtre window

	glfwSetKeyCallback(window, sortieClavier); // écoute les entrées de clavier

	gladLoadGL(); // chargement des fonction de glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { Erreur(2); return -1; } // vérifi que la librairie a bien tout chargé

	Shader shaderProgram("default.vert", "default.frag"); // génere l'objet shader en utilisant le default.vert et le default.frag

	VAO VAO1; // génere le VAO
	VAO1.Bind(); // et le lie

	VBO VBO1(vertices, sizeof(vertices)); // génere le VBO et le lie aux vertices
	EBO EBO1(indices, sizeof(indices)); // génere le EBO et le lie aux indices

	VAO1.LinkVBO(VBO1, 0); // lie le VBO au VAO
	// délie tout les objets pour eviter une erreur de modification
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	// ========= La boucle principale ========= //
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents(); // dit de faire attention aux évenements que va subire la fenêtre

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f); // efface le tampon et lui donne une couleur définie
		glClear(GL_COLOR_BUFFER_BIT); // applique le changement précédent

		shaderProgram.Activate(); // dit à OpenGL quel shaderProgram utiliser
		VAO1.Bind(); // lie le VAO pour que OpenGL sache l'utiliser

		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0); // dessine les éléments (type de forme, nombre d'éléments, type des indices, index des indices)

		glfwSwapBuffers(window); // échange les buffers
	}

	// destruction des objets créés
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();

	glfwDestroyWindow(window); // Fin de la fenêtre
	glfwTerminate(); // Fin de l'utilisation de glfw
	return 0;
}