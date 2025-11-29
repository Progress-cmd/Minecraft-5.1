#version 330 core // indique la version d'OpenGL

layout (location = 0) in vec3 aPos; // entrée de type vec3 qui utilise la disposition à l'emplacement 0

uniform float scale; //permet de changer la taille globale. uniform le rend accessible partout, même dans main.cpp

void main()
{
   gl_Position = vec4(aPos.x + aPos.x*scale, aPos.y + aPos.y*scale, aPos.z + aPos.z*scale, 1.0);
}