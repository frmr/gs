#pragma once

#include "gsCulture.hpp"
#include "gsLandTile.hpp"
#include "gsRandomRange.hpp"
#include "gsSpreader.hpp"

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
        gs::LandTilePtr SelectRandomTileFromVector(const vector<gs::LandTilePtr> tiles, gs::RandomRange<double>& randomIndex) const;

    public:
        bool Spread();

    public:
        CultureSpreader(const unsigned int speed, const gs::LandTilePtr origin, const shared_ptr<gs::Culture> value, const gs::LandTile::Terrain terrainPreference);
    };
}