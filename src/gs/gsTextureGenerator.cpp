#include "gsTextureGenerator.h"

gs::TextureGenerator::BiomeColorGroup::BiomeColorGroup(const gs::Color& color0, const gs::Color& color1, const gs::Color& color2, const gs::Color& color3)
    :   color0(color0), color1(color1), color2(color2), color3(color3)
{
}

gs::Color gs::TextureGenerator::SelectColorFromGroup(const gs::TextureGenerator::BiomeColorGroup& group, const double noiseVal)
{
    constexpr double limit0 = 0.2;
    constexpr double limit1 = 0.4;
    constexpr double limit2 = 0.6;

    if (noiseVal < limit0)
    {
        const double interp = noiseVal / limit0;
        return gs::Color(group.color0.r + (int) ((group.color1.r-group.color0.r) * interp),
                          group.color0.g + (int) ((group.color1.g-group.color0.g) * interp),
                          group.color0.b + (int) ((group.color1.b-group.color0.b) * interp));
    }
    else if (noiseVal < limit1)
    {
        const double interp = (noiseVal - limit0) / (limit1 - limit0);
        return gs::Color(group.color1.r + (int) ((group.color2.r-group.color1.r) * interp),
                          group.color1.g + (int) ((group.color2.g-group.color1.g) * interp),
                          group.color1.b + (int) ((group.color2.b-group.color1.b) * interp));
    }
    else if (noiseVal < limit2)
    {
        const double interp = (noiseVal - limit1) / (limit2 - limit1);

        return gs::Color(group.color2.r + (int) ((group.color3.r-group.color2.r) * interp),
                          group.color2.g + (int) ((group.color3.g-group.color2.g) * interp),
                          group.color2.b + (int) ((group.color3.b-group.color2.b) * interp));
    }
    else
    {
        return group.color3;
    }
}

double gs::TextureGenerator::SampleNoise(const gs::LandTile::Terrain terrain, const gs::Vec3d& coord) const
{
    if (terrain == gs::LandTile::Terrain::PLAINS)
    {
        return noise.ScaledOctaveNoise(coord.x, coord.y, coord.z, 8, 0.4, 0.003, 0.0, 1.0);
    }
    else if (terrain == gs::LandTile::Terrain::HILLS)
    {
        return noise.ScaledOctaveNoise(coord.x, coord.y, coord.z, 8, 0.4, 0.003, 0.0, 1.0);
    }
    else if (terrain == gs::LandTile::Terrain::MOUNTAINS)
    {
        return noise.ScaledOctaveNoise(coord.x, coord.y, coord.z, 8, 0.4, 0.003, 0.0, 1.0);
    }
    else
    {
        return 0.0;
    }
}

gs::Color gs::TextureGenerator::Sample(const gs::LandTile::Biome biome, const gs::LandTile::Terrain terrain, const gs::Vec3d& coord) const
{
    const gs::TextureGenerator::BiomeColorGroup colors[7] = {
        gs::TextureGenerator::BiomeColorGroup(gs::Color(255, 0, 255), gs::Color(255, 0, 255), gs::Color(255, 0, 255), gs::Color(255, 0, 255)),            //unassigned
        gs::TextureGenerator::BiomeColorGroup(gs::Color(0, 100, 255), gs::Color(0, 100, 255), gs::Color(0, 100, 255), gs::Color(0, 100, 255)),            //lake
        gs::TextureGenerator::BiomeColorGroup(gs::Color(230, 230, 230), gs::Color(230, 230, 230), gs::Color(230, 230, 230), gs::Color(230, 230, 230)),    //ice
        gs::TextureGenerator::BiomeColorGroup(gs::Color(77, 112, 77), gs::Color(113, 141, 113), gs::Color(120, 120, 120), gs::Color(230, 230, 230)),      //tundra
        gs::TextureGenerator::BiomeColorGroup(gs::Color(0, 100, 0), gs::Color(0, 50, 0), gs::Color(120, 120, 120), gs::Color(230, 230, 230)),             //grass
        gs::TextureGenerator::BiomeColorGroup(gs::Color(181, 145, 108), gs::Color(127, 102, 76), gs::Color(120, 120, 120), gs::Color(230, 230, 230)),     //semi arid
        gs::TextureGenerator::BiomeColorGroup(gs::Color(120, 80, 40), gs::Color(90, 60, 30), gs::Color(120, 120, 120), gs::Color(230, 230, 230))          //desert
    };
    return SelectColorFromGroup(colors[(int)biome], SampleNoise(terrain, coord));
}

gs::TextureGenerator::TextureGenerator()
    :   noise(0)
{
}
