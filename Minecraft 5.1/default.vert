#version 330 core // indique la version d'OpenGL

layout (location = 0) in vec3 aPos; // entrée de type vec3 qui utilise la disposition à l'emplacement 0

layout (location = 1) in vec2 aTex; // entrée de type vec2 qui utilise la disposition à l'emplacement 1


out vec2 texCoord; // défini une sortie

uniform float scale; //permet de changer la taille globale. uniform le rend accessible partout, même dans main.cpp


void main()
{
   gl_Position = vec4(aPos.x + aPos.x*scale, aPos.y + aPos.y*scale, aPos.z + aPos.z*scale, 1.0);
   texCoord = aTex; // affecte la valeur en sortie
}