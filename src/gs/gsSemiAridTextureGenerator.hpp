#pragma once

#include "gsColor.hpp"
#include "gsLandTile.hpp"
#include <vector>
#include "../FastNoise/FastNoise.h"

namespace gs
{
	class SemiAridTextureGenerator
	{
	private:
		static const std::vector<gs::Vec3d> colors;
		static const std::vector<float>		limits;

		FastNoise noiseA;
		FastNoise noiseB;

	public:
		gs::Color Sample(const gs::Vec3d& coord, const gs::LandTile::Terrain terrain);

		SemiAridTextureGenerator();
	};
}