#ifndef CHUNK_H
#define CHUNK_H

#include <glad/glad.h>
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Texture.h"
#include "Camera.h"
#include "shaderClass.h"

#include <vector>
#include <array>
#include <map>
#include <cstdlib>

class Chunk
{
protected:
	// Initialisation des structures
	struct Block { uint8_t type; };
	struct FaceData { std::array<glm::vec3, 4> verts; std::array<glm::vec2, 4> uvs; };

	// Initialisation des types de blocs
	std::map<int, std::array<int, 3>> blockTypes =
	{//id, {Up, Down, Sides}
	{1, {0, 0, 0}},    // Dirt
	{2, {1, 0, 2}},    // Grass
	{3, {3, 3, 3}},    // Stone
	{4, {4, 4, 4}}     // Wood
	};

	// Création des objets
	Texture bitmap;
	VAO VAOBloc;
	Shader shaderProgramBloc;
	VBO VBOBloc;
	EBO EBOBloc;

	// Tableau stockant les UVs de 
	std::array<glm::vec2, 4> uvArray;

	// Définition des faces d'un cube
	FaceData FACE_POS_X = {
	{
		glm::vec3(0.5f, -0.5f, -0.5f),
		glm::vec3(0.5f,  0.5f, -0.5f),
		glm::vec3(0.5f,  0.5f,  0.5f),
		glm::vec3(0.5f, -0.5f,  0.5f)
	},
	{
		glm::vec2(1.0f / 16 * 0,  1.0f / 16 * 15),
		glm::vec2(1.0f / 16 * 0,  1.0f / 16 * 16),
		glm::vec2(1.0f / 16 * 1,  1.0f / 16 * 16),
		glm::vec2(1.0f / 16 * 1,  1.0f / 16 * 15)
	}
	};
	FaceData FACE_NEG_X = {
	{
		glm::vec3(-0.5f, -0.5f,  0.5f),
		glm::vec3(-0.5f,  0.5f,  0.5f),
		glm::vec3(-0.5f,  0.5f, -0.5f),
		glm::vec3(-0.5f, -0.5f, -0.5f)
	},
	{
		glm::vec2(1.0f / 16 * 0,  1.0f / 16 * 15),
		glm::vec2(1.0f / 16 * 0,  1.0f / 16 * 16),
		glm::vec2(1.0f / 16 * 1,  1.0f / 16 * 16),
		glm::vec2(1.0f / 16 * 1,  1.0f / 16 * 15)
	}
	};
	FaceData FACE_POS_Y = {
	{
		glm::vec3(-0.5f,  0.5f, -0.5f),
		glm::vec3(-0.5f,  0.5f,  0.5f),
		glm::vec3(0.5f,  0.5f,  0.5f),
		glm::vec3(0.5f,  0.5f, -0.5f)
	},
	{
		glm::vec2(1.0f / 16 * 0,  1.0f / 16 * 15),
		glm::vec2(1.0f / 16 * 0,  1.0f / 16 * 16),
		glm::vec2(1.0f / 16 * 1,  1.0f / 16 * 16),
		glm::vec2(1.0f / 16 * 1,  1.0f / 16 * 15)
	}
	};
	FaceData FACE_NEG_Y = {
	{
		glm::vec3(-0.5f, -0.5f,  0.5f),
		glm::vec3(0.5f, -0.5f,  0.5f),
		glm::vec3(0.5f, -0.5f, -0.5f),
		glm::vec3(-0.5f, -0.5f, -0.5f)
	},
	{
		glm::vec2(1.0f / 16 * 0,  1.0f / 16 * 15),
		glm::vec2(1.0f / 16 * 0,  1.0f / 16 * 16),
		glm::vec2(1.0f / 16 * 1,  1.0f / 16 * 16),
		glm::vec2(1.0f / 16 * 1,  1.0f / 16 * 15)
	}
	};
	FaceData FACE_POS_Z = {
	{
		glm::vec3(0.5f, -0.5f,  0.5f),
		glm::vec3(0.5f,  0.5f,  0.5f),
		glm::vec3(-0.5f,  0.5f,  0.5f),
		glm::vec3(-0.5f, -0.5f,  0.5f)
	},
	{
		glm::vec2(1.0f / 16 * 0,  1.0f / 16 * 15),
		glm::vec2(1.0f / 16 * 0,  1.0f / 16 * 16),
		glm::vec2(1.0f / 16 * 1,  1.0f / 16 * 16),
		glm::vec2(1.0f / 16 * 1,  1.0f / 16 * 15)
	}
	};
	FaceData FACE_NEG_Z = {
	{
		glm::vec3(-0.5f, -0.5f, -0.5f),
		glm::vec3(-0.5f,  0.5f, -0.5f),
		glm::vec3(0.5f,  0.5f, -0.5f),
		glm::vec3(0.5f, -0.5f, -0.5f)
	},
	{
		glm::vec2(1.0f / 16 * 0,  1.0f / 16 * 15),
		glm::vec2(1.0f / 16 * 0,  1.0f / 16 * 16),
		glm::vec2(1.0f / 16 * 1,  1.0f / 16 * 16),
		glm::vec2(1.0f / 16 * 1,  1.0f / 16 * 15)
	}
	};
	
	FaceData faces[6] =
	{
		FACE_POS_X,
		FACE_NEG_X,
		FACE_POS_Y,
		FACE_NEG_Y,
		FACE_POS_Z,
		FACE_NEG_Z
	};

	// Dimention du chunk
	static const int CHUNK_X = 16;
	static const int CHUNK_Y = 128;
	static const int CHUNK_Z = 16;

	// Tableau stockant les blocs du chunk
	Block blocks[CHUNK_X * CHUNK_Y * CHUNK_Z];

	// Les coordonnées du chunk
	const int m_xChunk;
	const int m_yChunk;

	// Tableau de données pour le VBO et l'EBO
	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;

	// Fonctions membres
	bool isAir(int x, int y, int z);
	void addFace(std::vector<GLfloat>& v, std::vector<GLuint>& i, const FaceData& face, int x, int y, int z);
	void typeBloc(int id);


public:
	Chunk(const int xChunk, const int yChunk);
	void BindBloc(Camera& camera, GLFWwindow* window, bool verticeMode);
	void Delete();

	// génération du chunk
	void Generation();
};

#endif CHUNK_H