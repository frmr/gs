#include "gsBiomeTextureGenerator.h"

gs::Color gs::BiomeTextureGenerator::Sample( const gs::Vec3d& coord, const gs::LandTile::Biome biome, const gs::LandTile::Terrain terrain ) const
{
    if ( biome == gs::LandTile::Biome::DESERT )
    {
        if ( terrain == gs::LandTile::Terrain::PLAINS )
            return gs::Color( 232, 243, 10 );
        else if ( terrain == gs::LandTile::Terrain::HILLS )
            return gs::Color( 200, 208, 9 );
        else
            return gs::Color( 137, 143, 5 );
    }
    else if ( biome == gs::LandTile::Biome::GRASSLAND )
    {
        if ( terrain == gs::LandTile::Terrain::PLAINS )
            return gs::Color( 85, 222, 61 );
        else if ( terrain == gs::LandTile::Terrain::HILLS )
            return gs::Color( 55, 185, 32 );
        else
            return gs::Color( 35, 116, 20 );
    }
    else if ( biome == gs::LandTile::Biome::SEMI_ARID )
    {
        if ( terrain == gs::LandTile::Terrain::PLAINS )
            return gs::Color( 236, 162, 23 );
        else if ( terrain == gs::LandTile::Terrain::HILLS )
            return gs::Color( 183, 124, 15 );
        else
            return gs::Color( 128, 87, 11 );
    }
    else if ( biome == gs::LandTile::Biome::TUNDRA )
    {
        if ( terrain == gs::LandTile::Terrain::PLAINS )
            return gs::Color( 200, 200, 200 );
        else if ( terrain == gs::LandTile::Terrain::HILLS )
            return gs::Color( 150, 150, 150 );
        else
            return gs::Color( 100, 100, 100 );
    }
    else //unassigned
    {
        return gs::Color();
    }
}

gs::BiomeTextureGenerator::BiomeTextureGenerator()
{

}
