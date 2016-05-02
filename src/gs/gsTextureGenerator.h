#ifndef GS_TEXTURE_GENERATOR_H
#define GS_TEXTURE_GENERATOR_H

#include "gsColor.h"
#include "gsLandTile.h"
#include "../cck/cckSimplexNoise.h"

namespace gs
{
    class TextureGenerator
    {
    private:
        struct BiomeColorGroup
        {
        public:
            const gs::Color color0;
            const gs::Color color1;
            const gs::Color color2;
            const gs::Color color3;

        public:
            BiomeColorGroup(const gs::Color& color0, const gs::Color& color1, const gs::Color& color2, const gs::Color& color3);
        };

    private:
        const cck::SimplexNoise noise;

    private:
        static gs::Color    SelectColorFromGroup(const BiomeColorGroup& group, const double noiseVal);
        double              SampleNoise(const gs::LandTile::Terrain terrain, const gs::Vec3d& coord) const;

    public:
        gs::Color           Sample(const gs::LandTile::Biome biome, const gs::LandTile::Terrain terrain, const gs::Vec3d& coord) const;

    public:
        TextureGenerator();
    };
}

#endif
