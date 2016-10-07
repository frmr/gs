#pragma once

#include "gsLandTile.hpp"
#include "gsVec3.hpp"
#include "gsDesertTextureGenerator.hpp"
#include "gsGrasslandTextureGenerator.hpp"
#include "gsSemiAridTextureGenerator.hpp"
#include "gsTundraTextureGenerator.hpp"

#include <memory>

using std::shared_ptr;

namespace gs
{
    class LandTextureGenerator
    {
    private:
		static gs::DesertTextureGenerator		desert;
		static gs::GrasslandTextureGenerator	grassland;
		static gs::SemiAridTextureGenerator		semiArid;
		static gs::TundraTextureGenerator		tundra;


    public:
		gs::Color Sample(const gs::Vec3d& coord, const gs::LandTile::Biome biome, const gs::LandTile::Terrain terrain) const;

    public:
        LandTextureGenerator();
    };
}