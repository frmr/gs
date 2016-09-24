#ifndef GS_TUNDRA_TEXTURE_GENERATOR_H
#define GS_TUNDRA_TEXTURE_GENERATOR_H

#include "gsLandTile.h"
#include "gsColor.h"
#include <vector>
#include "../FastNoise/FastNoise.h"

namespace gs
{
	class TundraTextureGenerator
	{
	private:
		static const std::vector<gs::Vec3d> colors;
		static const std::vector<float>		limits;

		FastNoise noiseA;
		FastNoise noiseB;

	public:
		gs::Color Sample(const gs::Vec3d& coord, const gs::LandTile::Terrain terrain);

		TundraTextureGenerator();
	};
}

#endif