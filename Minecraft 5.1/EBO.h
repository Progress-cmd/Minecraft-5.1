#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include<glad/glad.h>

class EBO // c'est le Edge Buffer Object
{
public:
	GLuint ID; // identifiant de référence
	EBO(GLuint* indices, GLsizeiptr size); // contructeur

	void Bind(); // activation
	void Unbind(); // désactivation
	void Delete(); // destruction
};

#endif