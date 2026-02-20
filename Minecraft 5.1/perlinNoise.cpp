#include "perlinNoise.h"
#include <random>
#include <time.h>


Noise::Noise(int seed): m_seed(seed)
{
	srand(unsigned int(time(0)));
	m_table.resize(16 * 16);
}

Noise::~Noise()
{

}


float Noise::getValue(int x, int y)
{
	int incert = 5;
	return 128.0f - incert + (rand() % incert);
}
