#include <cmath>
#include <array>
#include <numeric>
#include <random>
#include <algorithm>
#include "Internal.h"
#include <Lumino/Graphics/PerlinNoise.h>

// http://mrl.nyu.edu/~perlin/Noise/

LN_NAMESPACE_BEGIN

static const float persistence = 0.5f;

//------------------------------------------------------------------------------
PerlinNoise::PerlinNoise(unsigned int seed)
{
	if (seed == 0) seed = std::mt19937::default_seed;

	// p[0..255] �𖄂߂ăV���b�t��
	for (int i = 0; i < 256; ++i) p[i] = i;
	std::shuffle(std::begin(p), std::begin(p) + 256, std::mt19937(seed));

	for (int i = 0; i<256; ++i)
	{
		p[256 + i] = p[i];
	}
}

//------------------------------------------------------------------------------
float PerlinNoise::Noise(float x) const
{
	return Noise(x, 0.0, 0.0);
}

//------------------------------------------------------------------------------
float PerlinNoise::Noise(float x, float y) const
{
	return Noise(x, y, 0.0);
}

//------------------------------------------------------------------------------
float PerlinNoise::Noise(float x, float y, float z) const
{
	int X = ((int)x) & 255;
	int Y = ((int)y) & 255;
	int Z = ((int)z) & 255;

	x = x - (int)x;
	y = y - (int)y;
	z = z - (int)z;

	float u = Fade(x);
	float v = Fade(y);
	float w = Fade(z);

	int A = p[X] + Y, AA = p[A] + Z, AB = p[A + 1] + Z;
	int B = p[X + 1] + Y, BA = p[B] + Z, BB = p[B + 1] + Z;

	return Lerp(w, Lerp(v, Lerp(u, Grad(p[AA], x, y, z),
		Grad(p[BA], x - 1, y, z)),
		Lerp(u, Grad(p[AB], x, y - 1, z),
			Grad(p[BB], x - 1, y - 1, z))),
		Lerp(v, Lerp(u, Grad(p[AA + 1], x, y, z - 1),
			Grad(p[BA + 1], x - 1, y, z - 1)),
			Lerp(u, Grad(p[AB + 1], x, y - 1, z - 1),
				Grad(p[BB + 1], x - 1, y - 1, z - 1))));
}

//------------------------------------------------------------------------------
float PerlinNoise::OctaveNoise(float x, int octaves) const
{
	float result = 0.0;
	float frequency = 1.0;
	float amplitude = 1.0;
	for (int i = 0; i < octaves; ++i)
	{
		result += Noise(x * frequency) * amplitude;
		amplitude *= persistence;
		frequency *= 2.0;
	}
	return result;
}

//------------------------------------------------------------------------------
float PerlinNoise::OctaveNoise(float x, float y, int octaves) const
{
	float result = 0.0;
	float frequency = 1.0;
	float amplitude = 1.0;
	for (int i = 0; i < octaves; ++i)
	{
		result += Noise(x * frequency, y * frequency) * amplitude;
		amplitude *= persistence;
		frequency *= 2.0;
	}
	return result;
}

//------------------------------------------------------------------------------
float PerlinNoise::OctaveNoise(float x, float y, float z, int octaves) const
{
	float result = 0.0;
	float frequency = 1.0;
	float amplitude = 1.0;
	for (int i = 0; i < octaves; ++i)
	{
		result += Noise(x * frequency, y * frequency, z * frequency) * amplitude;
		amplitude *= persistence;
		frequency *= 2.0;
	}
	return result;
}

//------------------------------------------------------------------------------
float PerlinNoise::Fade(float t) const
{
	return t * t * t * (t * (t * 6 - 15) + 10);
}

//------------------------------------------------------------------------------
float PerlinNoise::Lerp(float t, float a, float b) const
{
	return a + t * (b - a);
}

//------------------------------------------------------------------------------
float PerlinNoise::Grad(int hash, float x, float y, float z) const
{
	int h = hash & 15;
	float u = (h < 8) ? x : y;
	float v = (h < 4) ? y : (h == 12 || h == 14 ? x : z);
	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

LN_NAMESPACE_END
