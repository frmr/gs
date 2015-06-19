#ifndef GS_BIOME_SPREADER_H
#define GS_BIOME_SPREADER_H

#include "gsSpreader.h"
#include "gsLandTile.h"
#include "gsRandomRange.h"

namespace gs
{
    class BiomeSpreader : public gs::Spreader<gs::LandTilePtr>
    {
    private:
        gs::LandTile::Biome value;

    public:
        void CleanFrontier();

    public:
        bool Spread();

    public:
        BiomeSpreader( const unsigned int speed, const gs::LandTilePtr origin, const gs::LandTile::Biome value );
    };
}

#endif // GS_BIOME_SPREADER_H
