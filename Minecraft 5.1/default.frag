#version 330 core // indique la version d'OpenGL


out vec4 FragColor; // créé une couleur de type vec4


in vec2 texCoord; // défini une entrée


uniform sampler2D tex0; // indique l'unité de texture à utiliser


void main()
{
   FragColor = texture(tex0, texCoord);
}