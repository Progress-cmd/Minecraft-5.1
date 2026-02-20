#ifndef GENERATION_H
#define GENERATION_H

#include <vector>
#include <queue>
#include <mutex>
#include <atomic>
#include <thread>
#include <map>
#include <glm.hpp>

#include "Chunk.h"
#include "perlinNoise.h"


class Generation
{
protected:
    Shader* m_sharedShader;
    Texture* m_sharedTexture;
    Noise* m_sharedNoise;

    std::map<std::pair<int, int>, Chunk*> chunkMap;

    // On utilise RenderDistance pour le dessin et GenerationDistance pour la création
    int RenderDistance = 10;
    int GenerationDistance = 12;

    // Multithreading
    std::thread worker;
    std::mutex meshMutex;
    std::recursive_mutex chunkMutex;
    std::queue<std::pair<int, int>> meshRequests; // Coordonnées demandées
    std::queue<ChunkData> readyMeshes;           // Meshes prêts pour le GPU
    std::atomic<bool> running = false;

    void workerLoop();

public:
    Generation(int seed);
    ~Generation(); // Important pour stopper le thread proprement

    void start();
    void stop();

    void updateWorld(glm::vec3 playerPos);
    void updateMainThread(); // Upload GPU
    void draw(Camera& camera, bool wireframeMode); // Remplace ChunkBind

    void ChunkCreate(int cx, int cz);
    Chunk* getChunk(int cx, int cz);
    uint8_t getBlock(int x, int y, int z);

    void Delete(); // Nettoyage manuel si besoin
};

#endif