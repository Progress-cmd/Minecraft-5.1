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
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
//Fragment Shader source code
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";



// ======================================== //
// (((((((((((( LE CODE SOURCE )))))))))))) //
// ======================================== //

using namespace std;

// ============ Les variables ============= //
bool pleinEcran = false;
bool keyF11 = false;
GLfloat vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
};
GLuint indices[] = {
	0, 1, 2
};

// ============ Les fonctions ============= //
void Erreur(int value)
{
	switch (value)
	{
	case 0: cout << "ERREUR 0: GLFW n'a pas pu s'initialiser." << endl;
	case 1: cout << "ERREUR 1: La fenêtre n'a pas pu être créée." << endl;
	case 2: cout << "ERREUR 2: Glad n'a pas pu se charger" << endl;
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


	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER); // création du shader afin d'obtenir sa valeur par référence
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // (valeur de référence, nombre de chaine de caractère, source du shader, NULL) // attache le code source au shader
	glCompileShader(vertexShader); // compile le shader pour qu'il soit utilisable par la carte graphique

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // création du shader afin d'obtenir sa valeur par référence
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL); // (valeur de référence, nombre de chaine de caractère, source du shader, NULL) // attache le code source au shader
	glCompileShader(fragmentShader); // compile le shader pour qu'il soit utilisable par la carte graphique

	GLuint shaderProgram = glCreateProgram(); // création d'un programme pour encapsuler les shaders dedans
	glAttachShader(shaderProgram, vertexShader); // associe le vertex shader au programme
	glAttachShader(shaderProgram, fragmentShader); // associe le fragment shader au programme
	glLinkProgram(shaderProgram); // encapsulation du shaderProgram

	glDeleteShader(vertexShader); // supprime le vertex shader
	glDeleteShader(fragmentShader); // supprime le fragment shader


	// Création du VBO (Vertex Buffer Object) un tampon de sommets
	// Création du VAO (Vertex Array Object) un tableau de sommets
	// Création de l'EBO (Element Buffer Object) un tampon d'éléments
	GLuint VAO, VBO, EBO;

	glGenVertexArrays(1, &VAO); // (nombre d'objet, référence) génère un tableau d'objets et le stock dans la variable VAO
	glGenBuffers(1, &VBO); // (nombre d'objet, référence) génère un tampon d'objets et le stock dans la variable VBO
	glGenBuffers(1, &EBO); // (nombre d'objet, référence) génère un tampon d'objets et le stock dans la variable EBO

	glBindVertexArray(VAO); // liaison de l'objet afin qu'il devienne l'objet courant

	glBindBuffer(GL_ARRAY_BUFFER, VBO); // liaison de l'objet afin qu'il devienne l'objet courant
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // (type de tampon, taille des données, données, usage des données) stocke les données dans le tampon VBO

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // liaison de l'objet afin qu'il devienne l'objet courant
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // (type de tampon, taille des données, données, usage des données) stocke les données dans le tampon EBO

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // (position du premier argument, nombre de valeur par sommet, type, ..., pas, pointeur vers la position de début) configuration de VAO, permet la communication entre l'attribut de vertex et le shader vertex
	glEnableVertexAttribArray(0); // activation du VAO, zéro car c'est la position du vertexAttribPointeur

	glBindBuffer(GL_ARRAY_BUFFER, 0); // liaison de l'objet a 0 afin de le détacher de l'objet courant (précaution)
	glBindVertexArray(0); // liaison de l'objet a 0 afin de le détacher de l'objet courant (précaution)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // liaison de l'objet a 0 afin de le détacher de l'objet courant (précaution)


	// ========= La boucle principale ========= //
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents(); // dit de faire attention aux évenements que va subire la fenêtre


		glClearColor(0.07f, 0.13f, 0.17f, 1.0f); // efface le tampon et lui donne une couleur définie
		glClear(GL_COLOR_BUFFER_BIT); // applique le changement précédent

		glUseProgram(shaderProgram); // activation du shaderProgram
		glBindVertexArray(VAO); // lie le VAO afin de le rendre courant et d'indiquer qu'on veut l'utiliser

		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0); // dessine les éléments (type de forme, nombre d'éléments, type des indices, index des indices)

		glfwSwapBuffers(window); // échange les buffers
	}

	// destruction des objets créés pour plus de propreté
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);

	glfwDestroyWindow(window); // Fin de la fenêtre
	glfwTerminate(); // Fin de l'utilisation de glfw
	return 0;
}