#include "perlinNoise.h"
#include <random>
#include <algorithm>
#include <cmath>


Noise::Noise(int seed) : m_seed(seed)
{
	// Initialisation de ma table de permutation
	for (int i = 0; i < 256; i++)
	{
		m_permutationTable.push_back(i);
	}

	// J'initialise ce qui va mélanger ma table avec ma seed
	std::mt19937 machine(seed);

	// Je mélange ma table
	std::shuffle(m_permutationTable.begin(), m_permutationTable.end(), machine);

	// je double ma table pour éviter les débordements
	m_permutationTable.insert(m_permutationTable.end(), m_permutationTable.begin(), m_permutationTable.end());

	// Initialisation des vecteurs gradients possibles
	m_vectors.push_back({ 1.0f, 1.0f });
	m_vectors.push_back({ -1.0f, 1.0f });
	m_vectors.push_back({ 1.0f, -1.0f });
	m_vectors.push_back({ -1.0f, -1.0f });
}

Noise::~Noise()
{

}

float Noise::getValue(float x, float y)
{
	// On trouve les entiers inférieurs pour définir la cellule
	int xi = (int)std::floor(x);
	int yi = (int)std::floor(y);

	// Position relative (entre 0.0 et 1.0)
	float localX = x - std::floor(x);
	float localY = y - std::floor(y);

	// Mes vecteurs de calcul
	std::vector<Vector2> m_gradients;
	std::vector<Vector2> m_distances;

	// Les hashs de vecteurs gradients
	int hash00 = coordHash(xi, yi);
	int hash10 = coordHash(xi + 1, yi);
	int hash01 = coordHash(xi, yi + 1);
	int hash11 = coordHash(xi + 1, yi + 1);

	// Les vecteurs gradients
	m_gradients.push_back(m_vectors[hash00 % 4]);
	m_gradients.push_back(m_vectors[hash10 % 4]);
	m_gradients.push_back(m_vectors[hash01 % 4]);
	m_gradients.push_back(m_vectors[hash11 % 4]);

	m_distances.push_back({ localX,        localY });        // Coin Bas-Gauche (0,0)
	m_distances.push_back({ localX - 1.0f, localY });        // Coin Bas-Droit (1,0)
	m_distances.push_back({ localX,        localY - 1.0f }); // Coin Haut-Gauche (0,1)
	m_distances.push_back({ localX - 1.0f, localY - 1.0f }); // Coin Haut-Droit (1,1)

	// Calcul du produit scalaire entre le vecteur gradient et distance de chaque angle
	std::vector<float> m_influence;
	for (int i = 0; i < 4; i++)
	{
		float scalarProduct = m_gradients[i].x * m_distances[i].x + m_gradients[i].y * m_distances[i].y;
		m_influence.push_back(scalarProduct);
	}

	return interpolation(m_influence, localX, localY);
}

int Noise::coordHash(int x, int y)
{
	// hachage en cascade de Perlin
	int valeur1 = m_permutationTable[x & 255];
	return m_permutationTable[valeur1 + (y & 255)];
}

float Noise::LERP(float scalar1, float scalar2, float t)
{
	return scalar1 + t * (scalar2 - scalar1);
}

float Noise::Fade(float t)
{
	return float(6 * pow(t, 5) - 15 * pow(t, 4) + 10 * pow(t, 3));
}

float Noise::interpolation(std::vector<float>& m_influence, float x, float y)
{
	float u = Fade(x);
	float v = Fade(y);

	float bas = LERP(m_influence[0], m_influence[1], u);
	float haut = LERP(m_influence[2], m_influence[3], u);

	return LERP(bas, haut, v);
}

float Noise::getFractalNoise(float x, float y, int octaves, float persistence, float lacunarity) {
	float total = 0;
	float layerFreq = 1.0f;
	float layerAmp = 1.0f;
	float maxValue = 0;  // Utilisé pour normaliser le résultat final

	for (int i = 0; i < octaves; i++) {
		// On additionne les couches
		total += getValue(x * layerFreq, y * layerFreq) * layerAmp;

		maxValue += layerAmp;
		layerAmp *= persistence; // L'amplitude diminue à chaque octave
		layerFreq *= lacunarity;  // La fréquence augmente (plus de détails)
	}

	return total / maxValue; // On ramène le résultat entre -1 et 1
}