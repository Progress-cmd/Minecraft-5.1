#include "Chunk.h"

Chunk::Chunk(const int xChunk, const int yChunk) :
	shaderProgramBloc("default.vert", "default.frag"),
	VBOBloc(&vertices, vertices.size()),
	EBOBloc(&indices, indices.size()),
	bitmap("bitmap.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE),
	m_xChunk(xChunk),
	m_yChunk(yChunk)
{
	// Initialise tous les blocs à type = 1
	for (int x = 0; x < CHUNK_X; x++)
	{
		for (int y = 0; y < CHUNK_Y; y++)
		{
			for (int z = 0; z < CHUNK_Z; z++)
			{
				int idx = x + CHUNK_X * (z + CHUNK_Z * y);
				blocks[idx].type = 1;
			}
		}
	}

	// Initialisation des coordonnées de la texture de base (UV)
	uvArray[0] = glm::vec2(0.0f, 0.9375f);
	uvArray[1] = glm::vec2(0.0f, 1.0f);
	uvArray[2] = glm::vec2(0.0625f, 1.0f);
	uvArray[3] = glm::vec2(0.0625f, 0.9375f);

	// Initialisation du VAO et association de la texture
	VAOBloc.Bind();
	bitmap.texUnit(shaderProgramBloc, "tex0", 0);
	VAOBloc.Unbind();

	// Init des faces (ordre cohérent)
	faces[0] = FACE_POS_X;
	faces[1] = FACE_NEG_X;
	faces[2] = FACE_POS_Y;
	faces[3] = FACE_NEG_Y;
	faces[4] = FACE_POS_Z;
	faces[5] = FACE_NEG_Z;
}

void Chunk::Generation()
{
	for (int x = m_xChunk * 16; x < m_xChunk * 16 + 16; x++)
	{
		for (int y = 0; y < 128; y++)
		{
			for (int z = m_yChunk * 16; z < m_yChunk * 16 + 16; z++)
			{
				int lx = x - m_xChunk * 16;
				int lz = z - m_yChunk * 16;

				int idx = lx + CHUNK_X * (lz + CHUNK_Z * y);
				uint8_t b = blocks[idx].type;

				if (b == 0) continue;

				int tempType = blocks[idx].type;
				typeBloc(blockTypes[tempType][2]);
				FACE_POS_X.uvs = uvArray;
				FACE_NEG_X.uvs = uvArray;
				FACE_POS_Z.uvs = uvArray;
				FACE_NEG_Z.uvs = uvArray;
				typeBloc(blockTypes[tempType][0]);
				FACE_POS_Y.uvs = uvArray;
				typeBloc(blockTypes[tempType][1]);
				FACE_NEG_Y.uvs = uvArray;
				// Pour chaque face : check le voisin
				if (isAir(x + 1, y, z)) addFace(vertices, indices, FACE_POS_X, x, y, z);
				if (isAir(x - 1, y, z)) addFace(vertices, indices, FACE_NEG_X, x, y, z);
				if (isAir(x, y + 1, z)) addFace(vertices, indices, FACE_POS_Y, x, y, z);
				if (isAir(x, y - 1, z)) addFace(vertices, indices, FACE_NEG_Y, x, y, z);
				if (isAir(x, y, z + 1)) addFace(vertices, indices, FACE_POS_Z, x, y, z);
				if (isAir(x, y, z - 1)) addFace(vertices, indices, FACE_NEG_Z, x, y, z);
			}
		}
	}

	// liage du VAO, VBO, et EBO
	VAOBloc.Bind();
	VBOBloc.Bind();
	EBOBloc.Bind();

	// après avoir bindé VBOBloc et EBOBloc
	glBindBuffer(GL_ARRAY_BUFFER, VBOBloc.ID); // adapte si ton VBO expose un membre ID
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.empty() ? nullptr : vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOBloc.ID); // même remarque
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.empty() ? nullptr : indices.data(), GL_STATIC_DRAW);

	// configuration des attributs
	VAOBloc.LinkAttrib(VBOBloc, 0, 3, GL_FLOAT, 5 * sizeof(float), (void*)0); // lie le VBO au VAO
	VAOBloc.LinkAttrib(VBOBloc, 1, 2, GL_FLOAT, 5 * sizeof(float), (void*)(3 * sizeof(float))); // les coordonnées de la texture

	// délie tout les objets pour eviter une erreur de modification
	VAOBloc.Unbind();
	VBOBloc.Unbind();
	EBOBloc.Unbind();
}

void Chunk::typeBloc(int id)
{
	int col = id % 16;
	int row = id / 16;

	float u = col / 16.0f;
	float v = row / 16.0f;

	uvArray[0] = glm::vec2(u, 1.0f - v - 0.0625);
	uvArray[1] = glm::vec2(u, 1.0f - v);
	uvArray[2] = glm::vec2(u + 0.0625, 1.0f - v);
	uvArray[3] = glm::vec2(u + 0.0625, 1.0f - v - 0.0625);
}

bool Chunk::isAir(int x, int y, int z)
{
	// check limites du chunk
	if (x < m_xChunk * 16 || x >= m_xChunk * 16 + 16) return true;
	if (y < 0 || y >= 128) return true;
	if (z < m_yChunk * 16 || z >= m_yChunk * 16 + 16) return true;

	int lx = x - m_xChunk * 16;
	int lz = z - m_yChunk * 16;

	int idx = lx + CHUNK_X * (lz + CHUNK_Z * y);
	return blocks[idx].type == 0;
}

void Chunk::addFace(std::vector<GLfloat>& v, std::vector<GLuint>& i, const FaceData& face, int x, int y, int z)
{
	int startIndex = v.size() / 5;

	for (int n = 0; n < 4; n++)
	{
		v.push_back(face.verts[n].x + x);
		v.push_back(face.verts[n].y + y);
		v.push_back(face.verts[n].z + z);
		v.push_back(face.uvs[n].x);
		v.push_back(face.uvs[n].y);
	}

	i.push_back(startIndex + 0);
	i.push_back(startIndex + 1);
	i.push_back(startIndex + 2);
	i.push_back(startIndex + 2);
	i.push_back(startIndex + 3);
	i.push_back(startIndex + 0);
}


void Chunk::BindBloc(Camera& camera, GLFWwindow* window, bool verticeMode)
{
	shaderProgramBloc.Activate(); // dit à OpenGL quel shaderProgram utiliser
	bitmap.Bind();
	camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgramBloc, "camMatrix"); // créé et envoie les matrices aux shaders
	VAOBloc.Bind(); // lie le VAO pour que OpenGL sache l'utiliser
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0); // dessine les éléments (type de forme, nombre d'éléments, type des indices, index des indices)
	if (verticeMode)
	{
		glUniform1i(glGetUniformLocation(shaderProgramBloc.ID, "verticeMode"), true);
	}
	else
	{
		glUniform1i(glGetUniformLocation(shaderProgramBloc.ID, "verticeMode"), false);
	}
}

void Chunk::Delete()
{
	VAOBloc.Delete();
	VBOBloc.Delete();
	EBOBloc.Delete();
	shaderProgramBloc.Delete();
	bitmap.Delete();
}