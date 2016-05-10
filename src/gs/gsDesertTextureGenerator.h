#ifndef GS_DESERT_TEXTURE_GENERATOR_H
#define GS_DESERT_TEXTURE_GENERATOR_H

#include "gsColor.h"
#include "gsLandTile.h"
#include <vector>
#include "../FastNoise/FastNoise.h"

namespace gs
{
	class DesertTextureGenerator
	{
	private:
		static const std::vector<gs::Vec3d> colors;
		static const std::vector<float>		limits;

		FastNoise noiseA;
		FastNoise noiseB;

	public:
		gs::Color Sample(const gs::Vec3d& coord, const gs::LandTile::Terrain terrain);

		DesertTextureGenerator();
	};
}

#endif