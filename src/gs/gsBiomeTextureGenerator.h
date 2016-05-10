#ifndef GS_BIOME_TEXTURE_GENERATOR_H
#define GS_BIOME_TEXTURE_GENERATOR_H

#include "gsLandTile.h"
//#include "gsRandomRange.h"
//#include "gsTexture.h"
#include "gsVec3.h"
#include "gsDesertTextureGenerator.h"
#include "gsGrasslandTextureGenerator.h"

#include <memory>

using std::shared_ptr;

namespace gs
{
    class BiomeTextureGenerator
    {
    private:
		/*
        const shared_ptr<const gs::Texture> grassPlains;
        const shared_ptr<const gs::Texture> grassHills;
        const shared_ptr<const gs::Texture> grassMountains;

        const shared_ptr<const gs::Texture> semiAridPlains;
        const shared_ptr<const gs::Texture> semiAridHills;
        const shared_ptr<const gs::Texture> semiAridMountains;

        const shared_ptr<const gs::Texture> desertPlains;
        const shared_ptr<const gs::Texture> desertHills;
        const shared_ptr<const gs::Texture> desertMountains;

        const shared_ptr<const gs::Texture> tundraPlains;
        const shared_ptr<const gs::Texture> tundraHills;
        const shared_ptr<const gs::Texture> tundraMountains;
		
        const shared_ptr<const gs::Texture> unassigned;
        gs::RandomRange<int> randomOffset;
		*/

		static gs::DesertTextureGenerator	desert;
		static gs::GrasslandTextureGenerator	grassland;


    public:
        //gs::Color   Sample(const gs::Vec3d& coord, const gs::LandTile::Biome biome, const gs::LandTile::Terrain terrain) const;
        //gs::Vec2i                       GetRandomOffset();
        //shared_ptr<const gs::Texture>   GetTexture(const gs::LandTile::Biome biome, const gs::LandTile::Terrain terrain) const;
		gs::Color&	Sample(const gs::Vec3f& coord, const gs::LandTile::Biome biome, const gs::LandTile::Terrain terrain) const;

    public:
        BiomeTextureGenerator();
    };
}

#endif // GS_BIOME_TEXTURE_GENERATOR_H
