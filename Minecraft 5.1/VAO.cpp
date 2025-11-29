#include"VAO.h"

// Création du VAO (Vertex Array Object) un tableau de sommets
VAO::VAO()
{
	glGenVertexArrays(1, &ID); // (nombre d'objet, référence) génère un tableau d'objets et le stock dans la variable VAO
}

// Links a VBO to the VAO using a certain layout
void VAO::LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset)
{
	VBO.Bind();
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset); // (position du premier argument, nombre de valeur par sommet, type, ..., pas, pointeur vers la position de début) configuration de VAO, permet la communication entre l'attribut de vertex et le shader vertex
	glEnableVertexAttribArray(layout); // activation du VAO, zéro car c'est la position du vertexAttribPointeur
	VBO.Unbind();
}

// Binds the VAO
void VAO::Bind()
{
	glBindVertexArray(ID); // liaison de l'objet afin qu'il devienne l'objet courant
}

// Unbinds the VAO
void VAO::Unbind()
{
	// liaison de l'objet a 0 afin de le détacher de l'objet courant (précaution)
	glBindVertexArray(0);
}

// Deletes the VAO
void VAO::Delete()
{
	glDeleteVertexArrays(1, &ID);
}