#ifndef VBO_H
#define VBO_H

#include<glad/glad.h>
#include <vector>

class VBO // c'est le Vertex Buffer Object
{
public:
	GLuint ID; // identifiant de référence
	VBO(GLsizeiptr size);
	VBO(std::vector<GLfloat>* vertices, GLsizeiptr size); // contructeur
	void updateData(std::vector<GLfloat>& vertices);

	void Bind(); // activation
	void Unbind(); // désactivation
	void Delete(); // destruction
};

#endif