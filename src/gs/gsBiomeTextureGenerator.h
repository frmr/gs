#ifndef GS_BIOME_TEXTURE_GENERATOR_H
#define GS_BIOME_TEXTURE_GENERATOR_H

#include "gsLandTile.h"
#include "gsTexture.h"
#include "gsVec3.h"

namespace gs
{
    class BiomeTextureGenerator
    {
    private:

    public:
        gs::Color   Sample( const gs::Vec3d& coord, const gs::LandTile::Biome biome, const gs::LandTile::Terrain terrain ) const;

    public:
        BiomeTextureGenerator();
    };
}

#endif // GS_BIOME_TEXTURE_GENERATOR_H
