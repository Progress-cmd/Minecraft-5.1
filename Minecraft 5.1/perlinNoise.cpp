#include "perlinNoise.h"


Noise::Noise(int seed): m_seed(seed)
{
	m_table.resize(16 * 16);
	//m_table[i] = 100;
}

float Noise::getValue(int x, int y)
{
	return 100.0f;
}
