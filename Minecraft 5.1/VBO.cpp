#include"VBO.h"
#include<iostream>

// Création du VBO (Vertex Buffer Object) un tampon de sommets
VBO::VBO(GLfloat* vertices, GLsizeiptr size)
{
	glGenBuffers(1, &ID); // (nombre d'objet, référence) génère un tampon d'objets et le stock dans la variable VBO
	glBindBuffer(GL_ARRAY_BUFFER, ID); // liaison de l'objet afin qu'il devienne l'objet courant
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW); // (type de tampon, taille des données, données, usage des données) stocke les données dans le tampon VBO
}

// activation du VBO
void VBO::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

// désactivation du VBO
void VBO::Unbind()
{
	// liaison de l'objet a 0 afin de le détacher de l'objet courant (précaution)
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// destruction du VBO
void VBO::Delete()
{
	glDeleteBuffers(1, &ID);
}