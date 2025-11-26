#ifndef VBO_H
#define VBO_H

#include<glad/glad.h>

class VBO // c'est le Vertex Buffer Object
{
public:
	GLuint ID; // identifiant de référence
	VBO(GLfloat* vertices, GLsizeiptr size); // contructeur

	void Bind(); // activation
	void Unbind(); // désactivation
	void Delete(); // destruction
};

#endif