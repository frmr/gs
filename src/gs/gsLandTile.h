#ifndef GS_LAND_TILE_H
#define GS_LAND_TILE_H

#include "gsTile.h"
#include "gsRandomRange.h"
#include "gsTileTexture.h"
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

    public:
        const int                   regionId;
        const Terrain               terrain;

    private:
        bool                        forested;
        Biome                       biome;
        gs::TileTexture*            texture;


    private:
        Terrain                             DetermineTerrain() const;

    public:

        void                                GenerateTexture();
        vector<shared_ptr<gs::LandTile>>    GetUnassignedBiomeNeighbors() const;
        Biome                               GetBiome() const;
        bool                                HasUnassignedBiomeNeighbors() const;
        void                                SetBiome( const Biome newBiome );
        void                                SetBlackIfRiver(); //TODO: delete
        bool                                SpawnRiver( const int newRiverId, gs::RandomRange<double>& rand );

    public:
        LandTile( const int bufferOffset, const vector<shared_ptr<gs::Vertex>>& vertices, const gs::Vec3f& centroid, const double height, const int regionId );
    };

    typedef shared_ptr<gs::LandTile> LandTilePtr;
}

#endif // GS_LAND_TILE_H
