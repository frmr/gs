#ifndef GS_LAND_TILE_H
#define GS_LAND_TILE_H

#include "gsTile.h"
#include "gsRandomRange.h"
#include "gsTexture.h"
#include "gsVec2.h"
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
        shared_ptr<gs::Texture>     texture; //TODO: make sure to delete this when pushed to vram, and should be unique_ptr
        vector<gs::Vec2f>           texCoords;

        static gs::RandomRange<int> colorGenerator; //TODO: delete


    private:
        Terrain                             DetermineTerrain() const;

    public:
        void                                AddToTileGroupTexture( shared_ptr<gs::Texture> tileGroupTexture, const gs::Vec2i& tileGroupTextureOffset, const int tileGroupTextureSize );
        void                                DeleteLocalTextureData();
        void                                GenerateTexture();
        shared_ptr<gs::Texture>             GetTexture() const;
        vector<shared_ptr<gs::LandTile>>    GetUnassignedBiomeNeighbors() const;
        Biome                               GetBiome() const;
        bool                                HasUnassignedBiomeNeighbors() const;
        void                                SetBiome( const Biome newBiome );
        void                                SetBlackIfRiver(); //TODO: delete
        bool                                SpawnRiver( const int newRiverId, gs::RandomRange<double>& rand );
        void                                UpdateAllBuffers( const GLuint positionVbo, const GLuint colorVbo, const GLuint fogVbo, const GLuint texCoordVbo );
        void                                UpdateTexCoordBuffer( const GLuint texCoordVbo );

    public:
        LandTile( const vector<shared_ptr<gs::Vertex>>& vertices, const gs::Vec3f& centroid, const double height, const int regionId );
        ~LandTile();
    };

    typedef shared_ptr<gs::LandTile> LandTilePtr;
}

#endif // GS_LAND_TILE_H
