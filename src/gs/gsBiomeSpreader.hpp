#pragma once

#include "gsSpreader.hpp"
#include "gsLandTile.hpp"
#include "gsRandomRange.hpp"

namespace gs
{
    class BiomeSpreader : public gs::Spreader<gs::LandTilePtr>
    {
    private:
        gs::LandTile::Biome     value;
        gs::LandTile::Terrain   terrainPreference;

    private:
        void            CleanFrontier();
        gs::LandTilePtr SelectRandomTileFromVector(const vector<gs::LandTilePtr> tiles, gs::RandomRange<double>& randomIndex) const;

    public:
        bool Spread();

    public:
        BiomeSpreader(const unsigned int speed, const gs::LandTilePtr origin, const gs::LandTile::Biome value, const gs::LandTile::Terrain terrainPreference);
    };
}