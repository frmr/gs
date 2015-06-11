#include "gsLandTile.h"

gs::LandTile::Terrain gs::LandTile::DetermineTerrain() const
{
    if ( height < 0.25 )
    {
        return gs::LandTile::Terrain::PLAINS;
    }
    else if ( height < 0.7 )
    {
        return gs::LandTile::Terrain::HILLS;
    }
    else
    {
        return gs::LandTile::Terrain::MOUNTAINS;
    }
}

gs::LandTile::Biome gs::LandTile::GetBiome() const
{
    return biome;
}

bool gs::LandTile::HasUnassignedBiomeNeighbors() const
{
    for ( const auto& link : landLinks )
    {
        if ( link.target->GetBiome() == gs::LandTile::Biome::UNASSIGNED )
        {
            return true;
        }
    }
    return false;
}

void gs::LandTile::SetBiome( const gs::LandTile::Biome newBiome )
{
    biome = newBiome;
}

gs::LandTile::LandTile( const int bufferOffset, const vector<shared_ptr<gs::Vertex>>& vertices, const double height, const int regionId )
    :   gs::Tile( gs::Tile::Type::LAND, bufferOffset, vertices ),
        height( height ),
        regionId( regionId ),
        terrain( DetermineTerrain() )
{
    if ( terrain == gs::LandTile::Terrain::PLAINS )
    {
        color = gs::Vec3f( 0.0f, 0.8f, 0.0f );
    }
    else if ( terrain == gs::LandTile::Terrain::HILLS )
    {
        color = gs::Vec3f( 0.0f, 0.4f, 0.0f );
    }
    else if ( terrain == gs::LandTile::Terrain::MOUNTAINS )
    {
        color = gs::Vec3f( 0.4, 0.4, 0.4 );
    }
    //color = gs::Vec3f( 0.0f, ( 8.0f * (float) id ) / 255.0f, 0.0f );
}
