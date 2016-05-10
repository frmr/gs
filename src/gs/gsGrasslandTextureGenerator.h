#ifndef GS_GRASS_TEXTURE_GENERATOR_H
#define GS_GRASS_TEXTURE_GENERATOR_H

#include "gsColor.h"
#include "gsLandTile.h"
#include <vector>
#include "../FastNoise/FastNoise.h"

namespace gs
{
	class GrasslandTextureGenerator
	{
	private:
		static const std::vector<gs::Vec3f> colors;
		static const std::vector<float>		limits;

		FastNoise noiseA;
		FastNoise noiseB;

	public:
		gs::Color Sample(const gs::Vec3f& coord, const gs::LandTile::Terrain terrain);

		GrasslandTextureGenerator();
	};
}

#endif