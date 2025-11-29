#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include<glad/glad.h>
#include"VBO.h"

class VAO // c'est le Array Buffer Object
{
public:
	GLuint ID; // identifiant de référence
	VAO(); // contructeur

	void LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset); // lie le VBO au VAO
	void Bind(); // activation
	void Unbind(); // désactivation
	void Delete(); // destruction
};
#endif