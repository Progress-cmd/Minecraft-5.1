#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D textTexture;
uniform vec3 textColor;

void main() {    
    // On récupère l'intensité du rouge (le glyphe de FreeType)
    // .r correspond aux données de la texture générée par FreeType
    float sampled = texture(textTexture, TexCoords).r;
    
    // On crée un vecteur 4D : 
    // RGB = la couleur voulue
    // Alpha = l'intensité du rouge (0.0 = transparent, 1.0 = opaque)
    color = vec4(textColor, 1.0) * vec4(1.0, 1.0, 1.0, sampled);
}