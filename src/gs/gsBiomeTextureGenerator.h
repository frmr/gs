#ifndef GS_BIOME_TEXTURE_GENERATOR_H
#define GS_BIOME_TEXTURE_GENERATOR_H

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
    class BiomeTextureGenerator
    {
    private:
		static gs::DesertTextureGenerator		desert;
		static gs::GrasslandTextureGenerator	grassland;
		static gs::SemiAridTextureGenerator		semiArid;
		static gs::TundraTextureGenerator		tundra;


    public:
		gs::Color&	Sample(const gs::Vec3d& coord, const gs::LandTile::Biome biome, const gs::LandTile::Terrain terrain) const;

    public:
        BiomeTextureGenerator();
    };
}

#endif // GS_BIOME_TEXTURE_GENERATOR_H
