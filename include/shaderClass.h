#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H


#include<glad/glad.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>

std::string get_file_contents(const char* filename); // fonction qui nous permet de lire les fichiers shaders	

class Shader // Ce n'est qu'un shaderProgram, bien structuré
{
public:
	GLuint ID; // identifiant de référence
	Shader(const char* vertexFile, const char* fragmentFile); // constructeur

	void Activate(); // activation
	void Delete(); // destruction

private:
	void compileErrors(unsigned int shader, const char* type);
};

#endif