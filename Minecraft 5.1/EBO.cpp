#include"EBO.h"

// Création de l'EBO (Element Buffer Object) un tampon d'éléments
EBO::EBO(std::vector<GLuint>* indices, GLsizeiptr size)
{
	glGenBuffers(1, &ID); // (nombre d'objet, référence) génère un tampon d'objets et le stock dans la variable EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID); // liaison de l'objet afin qu'il devienne l'objet courant
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW); // (type de tampon, taille des données, données, usage des données) stocke les données dans le tampon EBO
}

// activation du EBO
void EBO::Bind()
{
	// lie le VAO afin de le rendre courant et d'indiquer qu'on veut l'utiliser
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

// désactivation du EBO
void EBO::Unbind()
{
	// liaison de l'objet a 0 afin de le détacher de l'objet courant (précaution)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// destruction du VBO
void EBO::Delete()
{
	glDeleteBuffers(1, &ID);
}