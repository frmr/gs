#include "gsLandTile.h"
#include "gsRandomRange.h"
#include "gsGlobe.h"

#include <ctime>

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

vector<shared_ptr<gs::LandTile>> gs::LandTile::GetUnassignedBiomeNeighbors() const
{
    vector<shared_ptr<gs::LandTile>> unassignedNeighbors;
    for ( const auto& link : landLinks )
    {
        if ( link.target->GetBiome() == gs::LandTile::Biome::UNASSIGNED )
        {
            unassignedNeighbors.push_back( link.target );
        }
    }
    return unassignedNeighbors;
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

void gs::LandTile::SetBlackIfRiver() //TODO: delete
{
//    for ( auto link : landLinks )
//    {
//        if ( link.edge->IsRiver() )
//        {
//            color = gs::Vec3f( 0.0, 0.0, 0.0 );
//        }
//    }

    if ( biome == gs::LandTile::Biome::DESERT )
    {
        if ( terrain == gs::LandTile::Terrain::PLAINS )
            color = gs::Vec3f( 232.0/255.0, 243.0/255.0, 10.0/255.0 );
        else if ( terrain == gs::LandTile::Terrain::HILLS )
            color = gs::Vec3f( 200.0/255.0, 208.0/255.0, 9.0/255.0 );
        else
            color = gs::Vec3f( 137.0/255.0, 143.0/255.0, 5.0/255.0 );
    }
    else if ( biome == gs::LandTile::Biome::GRASSLAND )
    {
        if ( terrain == gs::LandTile::Terrain::PLAINS )
            color = gs::Vec3f( 85.0/255.0, 222.0/255.0, 61.0/255.0 );
        else if ( terrain == gs::LandTile::Terrain::HILLS )
            color = gs::Vec3f( 55.0/255.0, 185.0/255.0, 32.0/255.0 );
        else
            color = gs::Vec3f( 35.0/255.0, 116.0/255.0, 20.0/255.0 );
    }
    else if ( biome == gs::LandTile::Biome::SEMI_ARID )
    {
        if ( terrain == gs::LandTile::Terrain::PLAINS )
            color = gs::Vec3f( 236.0/255.0, 162.0/255.0, 23.0/255.0 );
        else if ( terrain == gs::LandTile::Terrain::HILLS )
            color = gs::Vec3f( 183.0/255.0, 124.0/255.0, 15.0/255.0 );
        else
            color = gs::Vec3f( 128.0/255.0, 87.0/255.0, 11.0/255.0 );
    }
    else if ( biome == gs::LandTile::Biome::TUNDRA )
    {
        if ( terrain == gs::LandTile::Terrain::PLAINS )
            color = gs::Vec3f( 200.0/255.0, 200.0/255.0, 200.0/255.0 );
        else if ( terrain == gs::LandTile::Terrain::HILLS )
            color = gs::Vec3f( 150.0/255.0, 150.0/255.0, 150.0/255.0 );
        else
            color = gs::Vec3f( 100.0/255.0, 100.0/255.0, 100.0/255.0 );
    }
    else if ( biome == gs::LandTile::Biome::UNASSIGNED )
    {
        color = gs::Vec3f( 1.0f, 1.0f, 1.0f );
    }
}

bool gs::LandTile::SpawnRiver()
{
    double probability;
    if ( terrain == gs::LandTile::Terrain::PLAINS )
    {
        probability = 0.05;
    }
    else if ( terrain == gs::LandTile::Terrain::HILLS )
    {
        probability = 0.5;
    }
    else if ( terrain == gs::LandTile::Terrain::MOUNTAINS )
    {
        probability = 1.0;
    }

    gs::RandomRange<int> range( 0, vertices.size()-1, std::time( 0 ) );
    //pick random vertex
    vertices[range.Sample()]->SetRiver();
}

gs::LandTile::LandTile( const int bufferOffset, const vector<shared_ptr<gs::Vertex>>& vertices, const double height, const int regionId )
    :   gs::Tile( gs::Tile::Type::LAND, bufferOffset, vertices, height ),
        regionId( regionId ),
        terrain( DetermineTerrain() ),
        forested( false ),
        biome( gs::LandTile::Biome::UNASSIGNED )
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
