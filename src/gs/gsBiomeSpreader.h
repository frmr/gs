#ifndef GS_BIOME_SPREADER_H
#define GS_BIOME_SPREADER_H

#include "gsSpreader.h"
#include "gsLandTile.h"

namespace gs
{
    class BiomeSpreader : public gs::Spreader<shared_ptr<gs::LandTile>>
    {
    private:
        gs::LandTile::Biome value;

    public:
        bool Spread()
        {
            for ( unsigned int i = 0; i < speed; ++i )
            {
                //rand(0, frontier.size()-1)
                //tile->GetUnassignedNeighbor()->SetBiome( value )

                for ( auto& tile : frontier )
                {
                    if ( !tile->HasUnassignedBiomeNeighbors() )
                    {
                        //remove tile from frontier
                    }
                }
            }

            return !frontier.empty();
        }

    public:
        BiomeSpreader( const unsigned int speed, const shared_ptr<gs::LandTile> origin, const gs::LandTile::Biome value )
            :   Spreader<shared_ptr<gs::LandTile>>( speed, origin ),
                value( value )
        {
        }

    };
}

#endif // GS_BIOME_SPREADER_H
