#ifndef PERLIN_NOISE_H
#define PERLIN_NOISE_H

#include <vector>


class Noise
{
public:
	Noise(int seed);
	~Noise();

	float getFractalNoise(float x, float y, int octaves, float persistence, float lacunarity);

protected:
	int m_seed;

	std::vector<int> m_permutationTable;

	struct Vector2 {
		float x, y;
	};

	// Mes vecteurs de référence
	std::vector<Vector2> m_vectors;

	float getValue(float worldX, float worldY);
	int coordHash(int x, int y);
	float LERP(float scalar1, float scalar2, float t); // fonction d'interpolation
	float Fade(float t); // fonction de lissage
	float interpolation(std::vector<float>& m_influence, float x, float y); // interpolation
};


#endif