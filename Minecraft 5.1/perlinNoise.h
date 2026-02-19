#ifndef PERLIN_NOISE_H
#define PERLIN_NOISE_H

#include <vector>


class Noise
{
public:
	Noise(int seed);

	float getValue(int x, int y);

protected:
	int m_seed;

	std::vector<float> m_table;
};


#endif