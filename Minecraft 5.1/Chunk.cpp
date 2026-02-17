#include "Chunk.h"
#include "Generation.h"
#include "Texture.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Camera.h"

#include <map>
#include <array>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>


// On définit les 6 faces : PosX, NegX, PosY, NegY, PosZ, NegZ
// Chaque face a 4 sommets (x, y, z)
static const float STRIDE_VERTICES[6][4][3] = {
    // FACE_POS_X (+X)
    {{0.5f, -0.5f, -0.5f}, {0.5f,  0.5f, -0.5f}, {0.5f,  0.5f,  0.5f}, {0.5f, -0.5f,  0.5f}},
    // FACE_NEG_X (-X)
    {{-0.5f, -0.5f,  0.5f}, {-0.5f,  0.5f,  0.5f}, {-0.5f,  0.5f, -0.5f}, {-0.5f, -0.5f, -0.5f}},
    // FACE_POS_Y (+Y - TOP)
    {{-0.5f,  0.5f, -0.5f}, {-0.5f,  0.5f,  0.5f}, {0.5f,  0.5f,  0.5f}, {0.5f,  0.5f, -0.5f}},
    // FACE_NEG_Y (-Y - BOTTOM)
    {{-0.5f, -0.5f,  0.5f}, {0.5f, -0.5f,  0.5f}, {0.5f, -0.5f, -0.5f}, {-0.5f, -0.5f, -0.5f}},
    // FACE_POS_Z (+Z)
    {{0.5f, -0.5f,  0.5f}, {0.5f,  0.5f,  0.5f}, {-0.5f,  0.5f,  0.5f}, {-0.5f, -0.5f,  0.5f}},
    // FACE_NEG_Z (-Z)
    {{-0.5f, -0.5f, -0.5f}, {-0.5f,  0.5f, -0.5f}, {0.5f,  0.5f, -0.5f}, {0.5f, -0.5f, -0.5f}}
};

// Les UVs de base pour une face (0 à 1 en local sur la texture)
static const float STRIDE_UVS[4][2] = {
    {0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f}
};

static const std::map<int, std::array<int, 3>> BLOCK_TYPES =
{   //id, {Up, Down, Sides}
    {1, {0, 0, 0}},    // Dirt
    {2, {1, 0, 2}},    // Grass
    {3, {3, 3, 3}},    // Stone
    {4, {4, 4, 4}}     // Wood
};


Chunk::Chunk(int xChunk, int zChunk, Generation* world, Shader* m_sharedShader, Texture* m_sharedTexture) : m_xChunk(xChunk), m_yChunk(zChunk), m_world(world), m_shaderProgram(m_sharedShader), m_texture(m_sharedTexture)
{
    // 1. Initialisation du vecteur de blocs
    // On redimensionne le vecteur pour contenir tous les blocs du chunk (16*128*16)
    m_blocks.resize(WIDTH * HEIGHT * DEPTH);

    // Initialisation basique (tout en Dirt pour l'instant, comme ton ancien code)
    for (int i = 0; i < m_blocks.size(); i++) {
        m_blocks[i].type = 1;
    }

    m_vao = new VAO();
    m_vao->Bind();

    // On prépare le VBO/EBO mais on ne met pas de données encore (nullptr)
    m_vbo = new VBO(nullptr, 0); // Taille 0 pour l'instant
    m_ebo = new EBO(nullptr, 0);

    // Lier la texture au shader une fois pour toutes
    m_texture->texUnit(*m_shaderProgram, "tex0", 0);

    m_vao->Unbind();
    m_vbo->Unbind();
    m_ebo->Unbind();
}

Chunk::~Chunk()
{
    // Nettoyage de la mémoire (RAII manuel car on utilise des pointeurs bruts)
    m_vao->Delete(); delete m_vao;
    m_vbo->Delete(); delete m_vbo;
    m_ebo->Delete(); delete m_ebo;
}

// Récupère un bloc avec vérification des limites locales
uint8_t Chunk::getBlock(int localX, int localY, int localZ) const
{
    if (localX < 0 || localX >= WIDTH ||
        localY < 0 || localY >= HEIGHT ||
        localZ < 0 || localZ >= DEPTH)
        return 0; // Air par défaut si hors limites

    return m_blocks[getIndex(localX, localY, localZ)].type;
}

void Chunk::setBlock(int localX, int localY, int localZ, uint8_t type)
{
    if (localX >= 0 && localX < WIDTH &&
        localY >= 0 && localY < HEIGHT &&
        localZ >= 0 && localZ < DEPTH)
    {
        m_blocks[getIndex(localX, localY, localZ)].type = type;
        markDirty(); // Le mesh devra être reconstruit
    }
}

void Chunk::markDirty()
{
    m_isDirty = true;
}

ChunkData Chunk::buildMeshCPU()
{
    ChunkData data;
    data.cx = m_xChunk;
    data.cz = m_yChunk;

    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            for (int z = 0; z < DEPTH; z++) {
                uint8_t type = getBlock(x, y, z);
                if (type == 0) continue;

                // On vérifie chaque voisin avec notre nouvelle fonction
                if (shouldRenderFace(x + 1, y, z)) addFaceGeometry(data.vertices, data.indices, x, y, z, 0, type); // +X
                if (shouldRenderFace(x - 1, y, z)) addFaceGeometry(data.vertices, data.indices, x, y, z, 1, type); // -X
                if (shouldRenderFace(x, y + 1, z)) addFaceGeometry(data.vertices, data.indices, x, y, z, 2, type); // +Y
                if (shouldRenderFace(x, y - 1, z)) addFaceGeometry(data.vertices, data.indices, x, y, z, 3, type); // -Y
                if (shouldRenderFace(x, y, z + 1)) addFaceGeometry(data.vertices, data.indices, x, y, z, 4, type); // +Z
                if (shouldRenderFace(x, y, z - 1)) addFaceGeometry(data.vertices, data.indices, x, y, z, 5, type); // -Z
            }
        }
    }
    return data;
}

void Chunk::addFaceGeometry(std::vector<GLfloat>& vertices, std::vector<GLuint>& indices, int x, int y, int z, int faceDir, int blockType)
{
    // 1. Calculer l'index de départ pour les indices
    // (vertices contient 5 floats par sommet : x, y, z, u, v)
    GLuint startIndex = static_cast<GLuint>(vertices.size() / 5);

    // 2. Calculer l'ID de la Texture
    // faceDir: 0=Right, 1=Left (Side), 2=Top, 3=Bottom, 4=Front, 5=Back (Side)
    int texTypeIndex = 2; // Par défaut Side
    if (faceDir == 2) texTypeIndex = 0; // Top
    else if (faceDir == 3) texTypeIndex = 1; // Bottom

    // On récupère l'ID de texture depuis la map statique
    // Attention: il faut gérer le cas où le blockType n'est pas dans la map (crash prevention)
    int texID = 0;
    if (BLOCK_TYPES.find(blockType) != BLOCK_TYPES.end()) {
        texID = BLOCK_TYPES.at(blockType)[texTypeIndex];
    }

    // Calcul UV Atlas
    float uMin = (texID % 16) / 16.0f;
    float vMin = 1.0f - ((texID / 16) / 16.0f) - 0.0625f; // Inversion Y standard OpenGL

    // 3. Ajouter les 4 sommets de la face
    for (int n = 0; n < 4; n++) {
        // Position (Modèle + Monde)
        vertices.push_back(STRIDE_VERTICES[faceDir][n][0] + x);
        vertices.push_back(STRIDE_VERTICES[faceDir][n][1] + y);
        vertices.push_back(STRIDE_VERTICES[faceDir][n][2] + z);

        // UVs (Atlas)
        float uOffset = STRIDE_UVS[n][0] * 0.0625f;
        float vOffset = STRIDE_UVS[n][1] * 0.0625f;
        vertices.push_back(uMin + uOffset);
        vertices.push_back(vMin + vOffset);
    }

    // 4. Ajouter les indices (2 triangles pour faire un carré)
    indices.push_back(startIndex + 0);
    indices.push_back(startIndex + 1);
    indices.push_back(startIndex + 2);
    indices.push_back(startIndex + 2);
    indices.push_back(startIndex + 3);
    indices.push_back(startIndex + 0);
}

void Chunk::uploadMeshToGPU(const ChunkData& data)
{
    m_vao->Bind();

    // Mise à jour du VBO
    // Note: Dans ton code VBO.h, assure-toi d'avoir une méthode comme glBufferData
    // ou utilise directement les commandes OpenGL si ta classe VBO est limitée.
    // Ici je suppose que VBO a un constructeur ou une méthode update.
    // Comme on a initialisé VBO avec nullptr, on doit re-bind et envoyer les data.

    m_vbo->Bind();
    glBufferData(GL_ARRAY_BUFFER, data.vertices.size() * sizeof(GLfloat), data.vertices.data(), GL_STATIC_DRAW);

    m_ebo->Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.indices.size() * sizeof(GLuint), data.indices.data(), GL_STATIC_DRAW);

    // Attributs : 0 = Pos (3 floats), 1 = UV (2 floats) -> Total stride = 5 floats
    m_vao->LinkAttrib(*m_vbo, 0, 3, GL_FLOAT, 5 * sizeof(float), (void*)0);
    m_vao->LinkAttrib(*m_vbo, 1, 2, GL_FLOAT, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    m_vao->Unbind();
    m_vbo->Unbind();
    m_ebo->Unbind();

    m_isGenerated = true;
    m_indexCount = static_cast<int>(data.indices.size());
}

void Chunk::draw(Camera& camera, bool wireframeMode)
{
    if (!m_isGenerated || m_indexCount == 0) return;

    m_shaderProgram->Activate();
    m_texture->Bind();

    // Mise à jour de la matrice caméra
    // Assure-toi que les paramètres (FOV, near, far) correspondent à ta logique Camera.cpp
    camera.Matrix(45.0f, 0.1f, 100.0f, *m_shaderProgram, "camMatrix");

    // Position du Chunk dans le monde (Model Matrix)
    // Ici, le VBO contient des coordonnées locales (0..15). 
    // On doit déplacer le chunk à sa position m_xChunk * 16
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(m_xChunk * WIDTH, 0, m_yChunk * DEPTH));
    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram->ID, "model"), 1, GL_FALSE, glm::value_ptr(model));

    m_vao->Bind();

    // Mode fil de fer optionnel
    if (wireframeMode)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    m_ebo->Bind();
    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);

    m_vao->Unbind();
}

bool Chunk::shouldRenderFace(int x, int y, int z) const
{
    // 1. Si on dépasse les limites verticales (Y), c'est du vide, donc on dessine la face
    if (y < 0 || y >= HEIGHT) return true;

    // 2. Si on est à l'intérieur du chunk (entre 0 et 15 sur X et Z)
    if (x >= 0 && x < WIDTH && z >= 0 && z < DEPTH)
    {
        return getBlock(x, y, z) == 0;
    }

    // 3. Si on est sur le bord du chunk, on doit demander au MONDE (m_world)
    // car le voisin se trouve dans un AUTRE chunk.
    int worldX = m_xChunk * WIDTH + x;
    int worldZ = m_yChunk * DEPTH + z;

    return m_world->getBlock(worldX, y, worldZ) == 0;
}