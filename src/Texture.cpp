#include"Texture.h"

// création de l'objet Texture
Texture::Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType)
{
	// Assigns the type of the texture ot the texture object
	type = texType;

	int widthImg, heightImg, numColCh; // les informations de l'image
	stbi_set_flip_vertically_on_load(true); // afin d'avoir l'image à l'endroit
	unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0); // stockage de l'image chargée dans une chaîne de caractère

	glGenTextures(1, &ID); // génération de la texture, (nb texture, ptr vers la référence)
	glActiveTexture(slot); // activation de la texture, ce qui la met dans un emplacement spécifique
	glBindTexture(texType, ID); // liage de la texture

	glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR); // ajustage des paramètres de la texture
	glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // garde les pixels tel quel, sans en rajouter

	glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT); // ajustage des paramètres de la texture
	glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT); // répète si elle a de la place disponible autour

	glTexImage2D(texType, 0, GL_RGBA, widthImg, heightImg, 0, format, pixelType, bytes); // génération de la texture
	glGenerateMipmap(texType); // fct qui enregistre des versions plus petite de l'image de base

	stbi_image_free(bytes); // libération des données

	glBindTexture(texType, 0); // dissociation des textures pour éviter des erreurs de modification
}

// indique à notre uniform l'emplacement de la texture
void Texture::texUnit(Shader& shader, const char* uniform, GLuint unit)
{
	GLuint texUni = glGetUniformLocation(shader.ID, uniform); // permet d'affecter une valeur à "scale", qui se trouve dans le default.vert
	shader.Activate();
	glUniform1i(texUni, unit);
}

// activation de la Texture
void Texture::Bind()
{
	glBindTexture(type, ID);
}

// désactivation de la Texture
void Texture::Unbind()
{
	glBindTexture(type, 0);
}

// destruction de la Texture
void Texture::Delete()
{
	glDeleteTextures(1, &ID);
}