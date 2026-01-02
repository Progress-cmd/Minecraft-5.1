#ifndef POLICES_H
#define POLICES_H

#include <ft2build.h>
#include FT_FREETYPE_H
#include <iostream>
#include <glad/glad.h>
#include <glm.hpp>
#include <map>
#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

class Polices
{
protected :
    struct Character {
        unsigned int TextureID;  // ID de la texture OpenGL
        glm::ivec2   Size;       // Taille du glyphe
        glm::ivec2   Bearing;    // Décalage du haut à gauche par rapport à la ligne de base
        unsigned int Advance;    // Distance jusqu'au prochain caractère
    };

    std::map<char, Character> Characters;

    VAO textVAO;
    VBO textVBO;
	Shader textShader;

public :
	Polices();
    void useTextShader(int width, int height);
    void RenderText(std::string text, float x, float y, float scale);
    ~Polices();

};

#endif