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
	struct Block { uint8_t type; };
	struct FaceData { std::array<glm::vec3, 4> verts; std::array<glm::vec2, 4> uvs; };
	std::map<int, std::array<int, 3>> blockTypes = { // Up, Down, Sides
	{1, {0, 0, 0}},    // Dirt
	{2, {1, 0, 2}},    // Grass
	{3, {3, 3, 3}},    // Stone
	{4, {4, 4, 4}}     // Wood
	};

public:
	// Variables concernant les blocs
	Texture bitmap;
	VAO VAOBloc;
	Shader shaderProgramBloc;
	VBO VBOBloc;
	EBO EBOBloc;

	static const int CHUNK_X = 16;
	static const int CHUNK_Y = 128;
	static const int CHUNK_Z = 16;
	Block blocks[CHUNK_X * CHUNK_Y * CHUNK_Z];

	std::array<glm::vec2, 4> uvArray;

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


	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;

	Chunk();
	void BindBloc(Camera& camera, GLFWwindow* window);
	void Delete();


	// génération du chunk
	void Generation(int xChunk, int yChunk, int texIdUp, int texIdDown, int texIdSides);
	bool isAir(int x, int y, int z);
	void addFace(std::vector<GLfloat>& v, std::vector<GLuint>& i, const FaceData& face, int x, int y, int z);
	void typeBloc(int id);
};

#endif CHUNK_H