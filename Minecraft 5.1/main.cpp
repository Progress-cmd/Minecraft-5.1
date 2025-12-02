// ======================================== //
// ((((((((((((( LES INCLUDES ))))))))))))) //
// ======================================== //


// ============ Les librairie ============= //
#include <iostream> // Flux d'entrées / sorties
#include <glad/glad.h> // Utilisation d'OpenGL
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
bool verticeMode = false;
bool keyF11 = false;
bool keyF10 = false;

GLfloat vertices[] =
{
	-0.5,  0.5, -0.5,
	 0.5,  0.5, -0.5,
	 0.5, -0.5, -0.5,
	-0.5, -0.5, -0.5,
	-0.5,  0.5,  0.5,
	 0.5,  0.5,  0.5,
	 0.5, -0.5,  0.5,
	-0.5, -0.5,  0.5

	//0, 256, 0,
	//16, 256, 0,
	//16, 0, 0,
	//0, 0, 0,
	//0, 256, 256,
	//16, 256, 256,
	//16, 0, 16,
	//0, 0, 16
	// 
	//0, 0.00390625, 0,
	//0.0625, 0.00390625, 0,
	//0.0625, 0, 0,
	//0, 0, 0,
	//0, 0.00390625, 0.00390625,
	//0.0625, 0.00390625, 0.00390625,
	//0.0625, 0, 0.0625,
	//0, 0, 0.0625
};

GLfloat textures[] =
{
	0.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 0.0f
};

GLuint indices[] =
{
	3, 0, 1,  1, 2, 3,
	2, 1, 5,  5, 6, 2,
	6, 5, 4,  4, 7, 6,
	7, 4, 0,  0, 3, 7,
	0, 1, 5,  5, 4, 0,
	7, 6, 2,  2, 3, 7
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

void f10(GLFWwindow* window, int key, int action) {
	if (key == GLFW_KEY_F10 && action == GLFW_PRESS)
	{
		if (!keyF10)
			keyF10 = true;
		{
			if (verticeMode)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				verticeMode = false;
			}
			else
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				verticeMode = true;
			}
		}
	}
	else
		keyF10 = false;
}

void sortieClavier(GLFWwindow* window, int key, int scancode, int action, int mode) {
	escape(window, key, action);
	f11(window, key, action);
	f10(window, key, action);
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
	VBO VBO2(textures, sizeof(textures));
	EBO EBO1(indices, sizeof(indices)); // génere le EBO et le lie aux indices

	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 5 * sizeof(float), (void*)0); // lie le VBO au VAO
	VAO1.LinkAttrib(VBO2, 1, 2, GL_FLOAT, 5 * sizeof(float), (void*)0); // si il y a des couleurs par points
	// délie tout les objets pour eviter une erreur de modification
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale"); // permet d'affecter une valeur à "scale", qui se trouve dans le default.vert

	// Texture
	int widthImg, heightImg, nuColCh; // les informations de l'image
	unsigned char* bytes = stbi_load("bitmap.png", &widthImg, &heightImg, &nuColCh, 0); // stockage de l'image chargée dans une chaîne de caractère

	GLuint texture; // création de l'objet texture
	glGenTextures(1, &texture); // génération de la texture, (nb texture, ptr vers la référence)
	glActiveTexture(GL_TEXTURE0); // activation de la texture, ce qui la met dans un emplacement spécifique
	glBindTexture(GL_TEXTURE_2D, texture); // liage de la texture

	glTexParameteri(GL_TEXTURE0, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // ajustage des paramètres de la texture
	glTexParameteri(GL_TEXTURE0, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // garde les pixels tel quel, sans en rajouter

	glTexParameteri(GL_TEXTURE0, GL_TEXTURE_WRAP_S, GL_REPEAT); // ajustage des paramètres de la texture
	glTexParameteri(GL_TEXTURE0, GL_TEXTURE_WRAP_T, GL_REPEAT); // répète si elle a de la place disponible autour

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes); // génération de la texture
	glGenerateMipmap(GL_TEXTURE_2D); // fct qui enregistre des versions plus petite de l'image de base

	stbi_image_free(bytes); // libération des données
	glBindTexture(GL_TEXTURE_2D, 0); // dissociation des textures pour éviter des erreurs de modification

	GLuint tex0Uni = glGetUniformLocation(shaderProgram.ID, "tex0"); // indique à notre uniform l'emplacement de la texture
	shaderProgram.Activate();
	glUniform1i(tex0Uni, 0);


	// ========= La boucle principale ========= //
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents(); // dit de faire attention aux évenements que va subire la fenêtre
		
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f); // efface le tampon et lui donne une couleur définie
		glClear(GL_COLOR_BUFFER_BIT); // applique le changement précédent

		shaderProgram.Activate(); // dit à OpenGL quel shaderProgram utiliser
		glBindTexture(GL_TEXTURE_2D, texture); // liage de la texture
		glUniform1f(uniID, 0.5f); // affecte une valeur à "scale"
		VAO1.Bind(); // lie le VAO pour que OpenGL sache l'utiliser

		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0); // dessine les éléments (type de forme, nombre d'éléments, type des indices, index des indices)

		glfwSwapBuffers(window); // échange les buffers
	}

	// destruction des objets créés
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	glDeleteTextures(1, &texture);
	shaderProgram.Delete();

	glfwDestroyWindow(window); // Fin de la fenêtre
	glfwTerminate(); // Fin de l'utilisation de glfw
	return 0;
}