#ifndef CHUNK_H
#define CHUNK_H

#include <vector>
#include <glad/glad.h>
#include <atomic>
#include <glm.hpp>


class VAO;
class VBO;
class EBO;
class Texture;
class Shader;
class Camera;
class Generation;
class Noise;


struct ChunkData {
    int cx; // Coordonnée X du chunk
    int cz; // Coordonnée Z du chunk
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;
};


class Chunk
{
public:
    // Dimensions du Chunk (Statiques pour accès global ex: Chunk::WIDTH)
    static constexpr int WIDTH = 16;
    static constexpr int HEIGHT = 128;
    static constexpr int DEPTH = 16;

    Chunk(int xChunk, int zChunk, Generation* world, Shader* m_sharedShader, Texture* m_sharedTexture, Noise* m_sharedNoise);
    ~Chunk();

    // --- Gestion du Mesh et Rendu ---

    // Construit les vertices/indices sur le CPU
    ChunkData buildMeshCPU();

    // Envoie les données préparées vers la carte graphique
    void uploadMeshToGPU(const ChunkData& data);

    // Affiche le chunk
    void draw(Camera& camera, bool wireframeMode = false, int maxGeneration = 8);

    // --- Gestion des Blocs ---

    uint8_t getBlock(int localX, int localY, int localZ) const;
    void setBlock(int localX, int localY, int localZ, uint8_t type); // Si tu as besoin de modifier

    // Marque le chunk pour régénération lors de la prochaine frame
    void markDirty();
    bool isDirty() { return m_isDirty; }

    // Getters pour le Thread de Génération
    void setBeingBuilt(bool status) { m_isBeingBuilt = status; }
    bool isBeingBuilt() const { return m_isBeingBuilt; }
    bool isReadyToDraw() const { return m_isReadyToDraw; }

    // --- Getters ---
    int getX() const { return m_xChunk; }
    int getZ() const { return m_yChunk; }

private:
    // Structure interne pour un bloc (économise la mémoire)
    struct Block {
        uint8_t type = 0; // 0 = air par défaut
    };

    // --- Données Membres ---

    // Coordonnées du chunk dans le monde
    const int m_xChunk;
    const int m_yChunk;

    // Pointeur vers le monde (pour accéder aux voisins)
    Generation* m_world;

    // État du chunk
    bool m_isDirty = true;      // Le mesh doit être mis à jour

    std::atomic<bool> m_isBeingBuilt{ false };  // Le CPU travaille
    bool m_isReadyToDraw = false;               // Le GPU est prêt

    // Nombre d'indices
    int m_indexCount = 0;

    // Stockage des blocs (Aplatissement 3D -> 1D pour cache locality)
    std::vector<Block> m_blocks;

    // --- OpenGL Objects ---
    // Utilisation de pointeurs ou d'objets (selon ton implémentation de VAO/VBO)
    // Ici je garde tes objets tels quels, mais idéalement, ils devraient être des unique_ptr
    // ou tes classes doivent gérer correctement la copie/déplacement.
    VAO* m_vao;
    VBO* m_vbo;
    EBO* m_ebo;
    Shader* m_shaderProgram;
    Texture* m_texture;
    Noise* m_noise;

    std::atomic<bool> m_isGenerating{ false };

    // --- Fonctions utilitaires internes (Helper) ---

    // Vérifie si un bloc est transparent/air pour l'optimisation des faces
    bool shouldRenderFace(int x, int y, int z) const;

    // Ajoute une face spécifique au tableau de vertices
    // Note: FaceData n'est plus membre de la classe, mais passé en paramètre ou géré en interne
    void addFaceGeometry(std::vector<GLfloat>& vertices, std::vector<GLuint>& indices,
        int x, int y, int z, int faceDir, int blockType);

    // Récupère l'index 1D depuis les coordonnées 3D locales
    inline int getIndex(int x, int y, int z) const {
        return x + WIDTH * (z + DEPTH * y);
    }
};

#endif