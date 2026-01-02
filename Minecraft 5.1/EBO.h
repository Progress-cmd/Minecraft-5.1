#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include<glad/glad.h>
#include <vector>

class EBO // c'est le Edge Buffer Object
{
public:
	GLuint ID; // identifiant de référence
	EBO(std::vector<GLuint>* indices, GLsizeiptr size); // contructeur
	void updateData(std::vector<GLuint>& indices);

	void Bind(); // activation
	void Unbind(); // désactivation
	void Delete(); // destruction
};

#endif