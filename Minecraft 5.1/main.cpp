// ======================================== //
// ((((((((((((( LES INCLUDES ))))))))))))) //
// ======================================== //


// ============ Les librairie ============= //
#include <iostream> // Flux d'entrées / sorties
#include <glad/glad.h> // Utilisation d'OpenGL
#include <GLFW/glfw3.h> // Création et gestion de la fenêtre
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <stb_image.h> // Utilisation des textures
#include <chrono>


// ========= Les fichiers sources ========= //
#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Texture.h"
#include "Camera.h"
#include "Chunk.h"
#include "Generation.h"
#include "Inputs.h"



// ======================================== //
// (((((((((((( LE CODE SOURCE )))))))))))) //
// ======================================== //

using namespace std;
using namespace std::chrono;

// ============ Les variables ============= //
int width = 800, height = 800;

int frames = 0;
double fps = 0.0;

auto start = high_resolution_clock::now();


// ============ Les fonctions ============= //
void Erreurs(int value)
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

void escape(GLFWwindow* window, int key, int action)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE); // fermeture de la fenêtre
}

void sortieClavier(GLFWwindow* window, int key, int scancode, int action, int mode) {
	escape(window, key, action);
}


// =========== La fonction main =========== //
int main() {
	glfwInit(); // Initialisation de GLFW

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // donne la version d'OpenGL
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6); // c'est 4.6 sa version
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //la version core contient toute les fonctions modernes (!= compatibility)
	if (!glfwInit()) { Erreurs(0); return -1; } // vérifi que la librairie s'est bien initialisée

	GLFWwindow* window = glfwCreateWindow(width, height, "Minecraft", NULL, NULL); // création de la fenêtre
	if (window == NULL) { Erreurs(1); glfwTerminate(); return -1; } // vérifi que la window est bien créée
	glfwMakeContextCurrent(window); // dit à glfw d'utiliser la fenêtre window

	glfwSetKeyCallback(window, sortieClavier); // écoute les entrées de clavier

	gladLoadGL(); // chargement des fonctions de glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { Erreurs(2); return -1; } // vérifi que la librairie a bien tout chargée

	Inputs inputsInit;
	Generation generationInit;

	generationInit.ChunkGeneration();

	glEnable(GL_DEPTH_TEST); // permet de dire à OpenGL de tenir compte de la perspective lors de l'affichage des textures

	Camera camera(width, height, glm::vec3(0.0f, 130.0f, 0.0f)); // création de l'objet caméra

	// ========= La boucle principale ========= //
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents(); // dit de faire attention aux évenements que va subire la fenêtre
		
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f); // efface le tampon et lui donne une couleur définie
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // applique le changement précédent

		frames++;
		auto now = high_resolution_clock::now();
		double elapsed = duration<double>(now - start).count();
		if (elapsed >= 1.0) {
			fps = frames / elapsed;
			cout << "FPS : " << fps << endl;

			frames = 0;
			start = now;
			cout << "Pos :" << camera.getPosition()[0] << ", " << camera.getPosition()[1] << ", " << camera.getPosition()[2] << endl;
		}

		inputsInit.processInput(window, camera);

		generationInit.ChunkBind(camera, window, inputsInit.getVerticeMode());

		glfwSwapBuffers(window); // échange les buffers
	}

	// destruction des objets créés
	generationInit.Delete();

	glfwDestroyWindow(window); // Fin de la fenêtre
	glfwTerminate(); // Fin de l'utilisation de glfw
	return 0;
}