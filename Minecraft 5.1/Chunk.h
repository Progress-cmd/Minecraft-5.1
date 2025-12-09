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

class Chunk
{
protected:
	struct Block { uint8_t type; };
	struct FaceData { glm::vec3 verts[4]; glm::vec2 uvs[4]; };

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
	void Generation();
	bool isAir(int x, int y, int z);
	void addFace(std::vector<GLfloat>& v, std::vector<GLuint>& i, const FaceData& face, int x, int y, int z);
};

#endif CHUNK_H