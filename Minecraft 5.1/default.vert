#version 330 core // indique la version d'OpenGL

layout (location = 0) in vec3 aPos; // entrée de type vec3 qui utilise la disposition à l'emplacement 0

layout (location = 1) in vec2 aTex; // entrée de type vec2 qui utilise la disposition à l'emplacement 1


out vec2 texCoord; // défini une sortie

uniform mat4 camMatrix; // uniform des matrices qui composent la caméra
uniform mat4 model; // AJOUT : Pour positionner le chunk

void main()
{
   gl_Position = camMatrix * model * vec4(aPos, 1.0);
   texCoord = aTex; // affecte la valeur en sortie
}