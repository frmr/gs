#ifndef GS_LAND_TILE_H
#define GS_LAND_TILE_H

#include "gsTile.h"
#include "gsVertex.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::vector;

namespace gs
{
    class LandTile : public gs::Tile
    {
    public:
        enum class Biome
        {
            UNASSIGNED,
            LAKE,
            ICE,
            TUNDRA,
            GRASSLAND,
            SEMI_ARID,
            DESERT
        };

        enum class Terrain
        {
            PLAINS,
            HILLS,
            MOUNTAINS
        };

    private:
        const int       regionId;
        const Terrain   terrain;
        bool            forested;

        Biome           biome;


    private:
        Terrain DetermineTerrain() const;

    public:

        Biome   GetBiome() const;
        bool    HasUnassignedBiomeNeighbors() const;
        void    SetBiome( const Biome newBiome );

    public:
        LandTile( const int bufferOffset, const vector<shared_ptr<gs::Vertex>>& vertices, const double height, const int regionId );
    };

    typedef shared_ptr<gs::LandTile> LandTilePtr;
}

#endif // GS_LAND_TILE_H
