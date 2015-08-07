#ifndef GS_BIOME_TEXTURE_GENERATOR_H
#define GS_BIOME_TEXTURE_GENERATOR_H

#include "gsLandTile.h"
#include "gsTexture.h"
#include "gsVec3.h"

#include <memory>

using std::shared_ptr;

namespace gs
{
    class BiomeTextureGenerator
    {
    private:
        const shared_ptr<const gs::Texture> grassPlains;
        const shared_ptr<const gs::Texture> unassigned;

    public:
        //gs::Color   Sample( const gs::Vec3d& coord, const gs::LandTile::Biome biome, const gs::LandTile::Terrain terrain ) const;
        shared_ptr<const gs::Texture>    GetTexture( const gs::LandTile::Biome biome, const gs::LandTile::Terrain terrain ) const;

    public:
        BiomeTextureGenerator();
    };
}

#endif // GS_BIOME_TEXTURE_GENERATOR_H
