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


// ========= Les fichiers sources ========= //
#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Texture.h"
#include "Camera.h"
#include "Chunk.h"


// ======================================== //
// (((((((((((( LE CODE SOURCE )))))))))))) //
// ======================================== //

using namespace std;

// ============ Les variables ============= //
int width = 800, height = 800;



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

void sortieClavier(GLFWwindow* window, int key, int scancode, int action, int mode) {
	escape(window, key, action);
}



// =========== La fonction main =========== //
int main() {
	GLFWInit();
	if (!glfwInit()) { Erreurs(0); return -1; } // vérifi que la librairie s'est bien initialisée

	GLFWwindow* window = glfwCreateWindow(800, 800, "Minecraft", NULL, NULL); // création de la fenêtre
	if (window == NULL) { Erreurs(1); glfwTerminate(); return -1; } // vérifi que la window est bien créée
	glfwMakeContextCurrent(window); // dit à glfw d'utiliser la fenêtre window

	glfwSetKeyCallback(window, sortieClavier); // écoute les entrées de clavier

	gladLoadGL(); // chargement des fonctions de glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { Erreurs(2); return -1; } // vérifi que la librairie a bien tout chargée

	Chunk chunk(0, 0);
	Chunk chunk1(1, 0);
	Chunk chunk2(0, 1);
	Chunk chunk3(-1, 0);
	Chunk chunk4(0, -1);
	Chunk chunk5(1, 1);
	Chunk chunk6(-1, 1);
	Chunk chunk7(-1, -1);
	Chunk chunk8(1, -1);

	chunk.Generation();
	chunk1.Generation();
	chunk2.Generation();
	chunk2.Generation();
	chunk3.Generation();
	chunk4.Generation();
	chunk5.Generation();
	chunk6.Generation();
	chunk7.Generation();
	chunk8.Generation();

	glEnable(GL_DEPTH_TEST); // permet de dire à OpenGL de tenir compte de la perspective lors de l'affichage des textures

	Camera camera(width, height, glm::vec3(0.0f, 130.0f, 2.0f)); // création de l'objet caméra

	// ========= La boucle principale ========= //
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents(); // dit de faire attention aux évenements que va subire la fenêtre
		
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f); // efface le tampon et lui donne une couleur définie
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // applique le changement précédent

		camera.Inputs(window);
		
		chunk.BindBloc(camera, window);
		chunk1.BindBloc(camera, window);
		chunk2.BindBloc(camera, window);
		chunk2.BindBloc(camera, window);
		chunk3.BindBloc(camera, window);
		chunk4.BindBloc(camera, window);
		chunk5.BindBloc(camera, window);
		chunk6.BindBloc(camera, window);
		chunk7.BindBloc(camera, window);
		chunk8.BindBloc(camera, window);

		glfwSwapBuffers(window); // échange les buffers
	}

	// destruction des objets créés
	chunk.Delete();

	glfwDestroyWindow(window); // Fin de la fenêtre
	glfwTerminate(); // Fin de l'utilisation de glfw
	return 0;
}