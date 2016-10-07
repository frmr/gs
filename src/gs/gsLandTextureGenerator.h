#pragma once

#include "gsLandTile.h"
#include "gsVec3.h"
#include "gsDesertTextureGenerator.h"
#include "gsGrasslandTextureGenerator.h"
#include "gsSemiAridTextureGenerator.h"
#include "gsTundraTextureGenerator.h"

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