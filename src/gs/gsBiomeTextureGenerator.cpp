#include "gsBiomeTextureGenerator.h"

gs::Vec3f gs::BiomeTextureGenerator::Sample( const gs::Vec3d& coord, const gs::LandTile::Biome biome, const gs::LandTile::Terrain terrain ) const
{
    if ( biome == gs::LandTile::Biome::DESERT )
    {
        if ( terrain == gs::LandTile::Terrain::PLAINS )
            return gs::Vec3f( 232.0/255.0, 243.0/255.0, 10.0/255.0 );
        else if ( terrain == gs::LandTile::Terrain::HILLS )
            return gs::Vec3f( 200.0/255.0, 208.0/255.0, 9.0/255.0 );
        else
            return gs::Vec3f( 137.0/255.0, 143.0/255.0, 5.0/255.0 );
    }
    else if ( biome == gs::LandTile::Biome::GRASSLAND )
    {
        if ( terrain == gs::LandTile::Terrain::PLAINS )
            return gs::Vec3f( 85.0/255.0, 222.0/255.0, 61.0/255.0 );
        else if ( terrain == gs::LandTile::Terrain::HILLS )
            return gs::Vec3f( 55.0/255.0, 185.0/255.0, 32.0/255.0 );
        else
            return gs::Vec3f( 35.0/255.0, 116.0/255.0, 20.0/255.0 );
    }
    else if ( biome == gs::LandTile::Biome::SEMI_ARID )
    {
        if ( terrain == gs::LandTile::Terrain::PLAINS )
            return gs::Vec3f( 236.0/255.0, 162.0/255.0, 23.0/255.0 );
        else if ( terrain == gs::LandTile::Terrain::HILLS )
            return gs::Vec3f( 183.0/255.0, 124.0/255.0, 15.0/255.0 );
        else
            return gs::Vec3f( 128.0/255.0, 87.0/255.0, 11.0/255.0 );
    }
    else if ( biome == gs::LandTile::Biome::TUNDRA )
    {
        if ( terrain == gs::LandTile::Terrain::PLAINS )
            return gs::Vec3f( 200.0/255.0, 200.0/255.0, 200.0/255.0 );
        else if ( terrain == gs::LandTile::Terrain::HILLS )
            return gs::Vec3f( 150.0/255.0, 150.0/255.0, 150.0/255.0 );
        else
            return gs::Vec3f( 100.0/255.0, 100.0/255.0, 100.0/255.0 );
    }
}

gs::BiomeTextureGenerator::BiomeTextureGenerator()
{

}
