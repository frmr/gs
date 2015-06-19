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

bool gs::BiomeSpreader::Spread()
{
    gs::RandomRange<double> randomIndex( 0.0, 0.9999, std::time( 0 ) );

    for ( unsigned int i = 0; i < speed; ++i )
    {
        if ( frontier.empty() )
        {
            return false;
        }

        int index = (int) ( randomIndex.Sample() * (double) frontier.size() );
        gs::LandTilePtr newTile = frontier[index]->GetUnassignedBiomeNeighbor();
        if ( newTile != nullptr ) //TODO: prevent this all together
        {
            newTile->SetBiome( value );
            frontier.push_back( newTile );
        }
        CleanFrontier();
    }

    return true;
}

gs::BiomeSpreader::BiomeSpreader( const unsigned int speed, const gs::LandTilePtr origin, const gs::LandTile::Biome value )
    :   Spreader<shared_ptr<gs::LandTile>>( speed, origin ),
        value( value )
{
    origin->SetBiome( value );
    CleanFrontier();
}
