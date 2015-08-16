#include "gsBiomeSpreader.h"

#include <ctime>
#include <iostream>

using std::cerr;
using std::endl;

void gs::BiomeSpreader::CleanFrontier()
{
    for ( vector<gs::LandTilePtr>::iterator it = frontier.begin(); it != frontier.end(); )
    {
        if ( !(*it)->HasUnassignedBiomeNeighbors() )
        {
            it = frontier.erase( it );
        }
        else
        {
            ++it;
        }
    }
}

gs::LandTilePtr gs::BiomeSpreader::SelectRandomTileFromVector( const vector<gs::LandTilePtr> tiles, gs::RandomRange<double>& randomIndex ) const
{
    return ( tiles.empty() ) ? nullptr : tiles[(int) ( randomIndex.Sample() * (double) tiles.size() )];
}

bool gs::BiomeSpreader::Spread()
{
    for ( unsigned int i = 0; i < speed; ++i )
    {
        if ( frontier.empty() )
        {
            return false;
        }

        vector<gs::LandTilePtr> unassignedNeighbors;

        for ( const auto& tile : frontier )
        {
            vector<gs::LandTilePtr> tileUnassignedNeighbors = tile->GetUnassignedBiomeNeighbors();
            unassignedNeighbors.insert( unassignedNeighbors.end(), tileUnassignedNeighbors.begin(), tileUnassignedNeighbors.end() );
        }

        vector<gs::LandTilePtr> unassignedPlains;
        vector<gs::LandTilePtr> unassignedHills;
        vector<gs::LandTilePtr> unassignedMountains;

        for ( const auto& tile : unassignedNeighbors )
        {
            if ( tile->terrain == gs::LandTile::Terrain::PLAINS )
            {
                unassignedPlains.push_back( tile );
            }
            else if ( tile->terrain == gs::LandTile::Terrain::HILLS )
            {
                unassignedHills.push_back( tile );
            }
            else if ( tile->terrain == gs::LandTile::Terrain::MOUNTAINS )
            {
                unassignedMountains.push_back( tile );
            }
        }

        //select best tile from the three vectors
        gs::RandomRange<double> randomIndex( 0.0, 0.9999, std::time( 0 ) );

        gs::LandTilePtr plainsTile = SelectRandomTileFromVector( unassignedPlains, randomIndex );
        gs::LandTilePtr hillsTile = SelectRandomTileFromVector( unassignedHills, randomIndex );
        gs::LandTilePtr mountainsTile = SelectRandomTileFromVector( unassignedMountains, randomIndex );

        gs::LandTilePtr bestTile = nullptr;

        if ( terrainPreference == gs::LandTile::Terrain::PLAINS )
        {
            if ( mountainsTile != nullptr )
            {
                bestTile = mountainsTile;
            }
            if ( hillsTile != nullptr )
            {
                bestTile = hillsTile;
            }
            if ( plainsTile != nullptr )
            {
                bestTile = plainsTile;
            }
        }
        else if ( terrainPreference == gs::LandTile::Terrain::HILLS )
        {
            if ( mountainsTile != nullptr )
            {
                bestTile = mountainsTile;
            }
            if ( plainsTile != nullptr )
            {
                bestTile = plainsTile;
            }
            if ( hillsTile != nullptr )
            {
                bestTile = hillsTile;
            }
        }
        else if ( terrainPreference == gs::LandTile::Terrain::MOUNTAINS )
        {
            if ( plainsTile != nullptr )
            {
                bestTile = plainsTile;
            }
            if ( hillsTile != nullptr )
            {
                bestTile = hillsTile;
            }
            if ( mountainsTile != nullptr )
            {
                bestTile = mountainsTile;
            }
        }

        if ( bestTile != nullptr ) //TODO: prevent nullptr all together
        {
            bestTile->SetBiome( value );
            frontier.push_back( bestTile );
        }
        else
        {
            return false;
        }

        CleanFrontier();

    }

    return true;
}

gs::BiomeSpreader::BiomeSpreader( const unsigned int speed, const gs::LandTilePtr origin, const gs::LandTile::Biome value, const gs::LandTile::Terrain terrainPreference )
    :   Spreader<shared_ptr<gs::LandTile>>( speed, origin ),
        value( value ),
        terrainPreference( terrainPreference )
{
    origin->SetBiome( value );
    CleanFrontier();
}
