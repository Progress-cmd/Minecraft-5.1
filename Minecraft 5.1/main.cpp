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


// ======================================== //
// (((((((((((( LE CODE SOURCE )))))))))))) //
// ======================================== //

using namespace std;

// ============ Les variables ============= //
int width = 800, height = 800;

GLfloat vertices[] =
{
	// Face avant
	-0.5f, -0.5f, -0.5f,   1.0f / 16 * 0,  1.0f / 16 * 15,
	-0.5f,  0.5f, -0.5f,   1.0f / 16 * 0,  1.0f / 16 * 16,
	 0.5f,  0.5f, -0.5f,   1.0f / 16 * 1,  1.0f / 16 * 16,
	 0.5f, -0.5f, -0.5f,   1.0f / 16 * 1,  1.0f / 16 * 15,

	 // Face droite
	 0.5f, -0.5f, -0.5f,   1.0f / 16 * 0,  1.0f / 16 * 15,
	 0.5f,  0.5f, -0.5f,   1.0f / 16 * 0,  1.0f / 16 * 16,
	 0.5f,  0.5f,  0.5f,   1.0f / 16 * 1,  1.0f / 16 * 16,
	 0.5f, -0.5f,  0.5f,   1.0f / 16 * 1,  1.0f / 16 * 15,

	 // Face arrière
	  0.5f, -0.5f,  0.5f,   1.0f / 16 * 0,  1.0f / 16 * 15,
	  0.5f,  0.5f,  0.5f,   1.0f / 16 * 0,  1.0f / 16 * 16,
	 -0.5f,  0.5f,  0.5f,   1.0f / 16 * 1,  1.0f / 16 * 16,
	 -0.5f, -0.5f,  0.5f,   1.0f / 16 * 1,  1.0f / 16 * 15,

	 // Face gauche
	-0.5f, -0.5f,  0.5f,   1.0f / 16 * 0,  1.0f / 16 * 15,
	-0.5f,  0.5f,  0.5f,   1.0f / 16 * 0,  1.0f / 16 * 16,
	-0.5f,  0.5f, -0.5f,   1.0f / 16 * 1,  1.0f / 16 * 16,
	-0.5f, -0.5f, -0.5f,   1.0f / 16 * 1,  1.0f / 16 * 15,

	  // Face du haut
	-0.5f,  0.5f, -0.5f,   1.0f / 16 * 0,  1.0f / 16 * 15,
	-0.5f,  0.5f,  0.5f,   1.0f / 16 * 0,  1.0f / 16 * 16,
	 0.5f,  0.5f,  0.5f,   1.0f / 16 * 1,  1.0f / 16 * 16,
	 0.5f,  0.5f, -0.5f,   1.0f / 16 * 1,  1.0f / 16 * 15,

	  // Face du bas
	-0.5f, -0.5f,  0.5f,   1.0f / 16 * 0,  1.0f / 16 * 15,
	 0.5f, -0.5f,  0.5f,   1.0f / 16 * 0,  1.0f / 16 * 16,
	 0.5f, -0.5f, -0.5f,   1.0f / 16 * 1,  1.0f / 16 * 16,
	-0.5f, -0.5f, -0.5f,   1.0f / 16 * 1,  1.0f / 16 * 15
};

//0, 256, 0,
//16, 256, 0,
//16, 0, 0,
//0, 0, 0,
//0, 256, 256,
//16, 256, 256,
//16, 0, 16,
//0, 0, 16

GLuint indices[] =
{
	0, 1, 2,   2, 3, 0,       // avant
	4, 5, 6,   6, 7, 4,       // droite
	8, 9,10,  10,11, 8,       // arrière
   12,13,14,  14,15,12,       // gauche
   16,17,18,  18,19,16,       // haut
   20,21,22,  22,23,20        // bas
};


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

	Shader shaderProgram("default.vert", "default.frag"); // génere l'objet shader en utilisant le default.vert et le default.frag

	VAO VAO1; // génere le VAO
	VAO1.Bind(); // et le lie

	VBO VBO1(vertices, sizeof(vertices)); // génere le VBO et le lie aux vertices
	EBO EBO1(indices, sizeof(indices)); // génere le EBO et le lie aux indices

	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 5 * sizeof(float), (void*)0); // lie le VBO au VAO
	VAO1.LinkAttrib(VBO1, 1, 2, GL_FLOAT, 5 * sizeof(float), (void*)(3 * sizeof(float))); // si il y a des couleurs par points
	// délie tout les objets pour eviter une erreur de modification
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	Texture bitmap("bitmap.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE); // création de l'objet bitmap
	bitmap.texUnit(shaderProgram, "tex0", 0);

	glEnable(GL_DEPTH_TEST); // permet de dire à OpenGL de tenir compte de la perspective lors de l'affichage des textures

	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f)); // création de l'objet caméra

	// ========= La boucle principale ========= //
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents(); // dit de faire attention aux évenements que va subire la fenêtre
		
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f); // efface le tampon et lui donne une couleur définie
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // applique le changement précédent

		shaderProgram.Activate(); // dit à OpenGL quel shaderProgram utiliser

		camera.f10(window, shaderProgram, "verticeMode"); // permet le mode vertice
		camera.f11(window); // permet le mode plein écran
		camera.Inputs(window);
		camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix"); // créé et envoie les matrices aux shaders

		bitmap.Bind(); // liage de la texture
		VAO1.Bind(); // lie le VAO pour que OpenGL sache l'utiliser

		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0); // dessine les éléments (type de forme, nombre d'éléments, type des indices, index des indices)

		glfwSwapBuffers(window); // échange les buffers
	}

	// destruction des objets créés
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	bitmap.Delete();
	shaderProgram.Delete();

	glfwDestroyWindow(window); // Fin de la fenêtre
	glfwTerminate(); // Fin de l'utilisation de glfw
	return 0;
}