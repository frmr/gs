#pragma once

#include "gsLandTile.hpp"
#include "gsColor.hpp"

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