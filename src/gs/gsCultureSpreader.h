#ifndef GS_CULTURE_SPREADER_H
#define GS_CULTURE_SPREADER_H

#include "gsCulture.h"
#include "gsLandTile.h"
#include "gsRandomRange.h"
#include "gsSpreader.h"

#include <memory>

using std::shared_ptr;

namespace gs
{
    class CultureSpreader : public gs::Spreader<gs::LandTilePtr>
    {
    private:
        shared_ptr<gs::Culture> value;
        gs::LandTile::Terrain   terrainPreference;

    private:
        void            CleanFrontier();
        gs::LandTilePtr SelectRandomTileFromVector( const vector<gs::LandTilePtr> tiles, gs::RandomRange<double>& randomIndex ) const;

    public:
        bool Spread();

    public:
        CultureSpreader( const unsigned int speed, const gs::LandTilePtr origin, const shared_ptr<gs::Culture> value, const gs::LandTile::Terrain terrainPreference );
    };
}

#endif // GS_CULTURE_SPREADER_H
