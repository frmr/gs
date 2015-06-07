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
        bool    forested;
        Biome   biome;
        Terrain terrain;

    private:
        static Terrain DetermineTerrain( const double height );

    public:

        Biome   GetBiome() const;
        bool    HasUnassignedBiomeNeighbors() const;
        void    SetBiome( const Biome newBiome );

    public:
        LandTile( const int bufferOffset, const vector<shared_ptr<gs::Vertex>>& vertices, const double height, const int id );

    };
}

#endif // GS_LAND_TILE_H
