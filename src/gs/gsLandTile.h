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
        gs::Texture*                texture; //TODO: make sure to delete this when pushed to vram
        gs::Vec2f                   textureCoords;

        static gs::RandomRange<int> colorGenerator; //TODO: delete


    private:
        Terrain                             DetermineTerrain() const;
        void                                InitTexCoordBuffer( const GLuint texCoordVbo );

    public:
        void                                AddToTileGroupTexture( gs::Texture* tileGroupTexture, const gs::Vec2i& coord ) const;
        void                                DeleteLocalTextureData();
        void                                GenerateTexture();
        gs::Texture*                        GetTexture() const;
        vector<shared_ptr<gs::LandTile>>    GetUnassignedBiomeNeighbors() const;
        Biome                               GetBiome() const;
        bool                                HasUnassignedBiomeNeighbors() const;
        void                                InitBuffers( const GLuint positionVbo, const GLuint colorVbo, const GLuint fogVbo, const GLuint texCoordVbo );
        void                                SetBiome( const Biome newBiome );
        void                                SetBlackIfRiver(); //TODO: delete
        bool                                SpawnRiver( const int newRiverId, gs::RandomRange<double>& rand );

    public:
        LandTile( const vector<shared_ptr<gs::Vertex>>& vertices, const gs::Vec3f& centroid, const double height, const int regionId );
    };

    typedef shared_ptr<gs::LandTile> LandTilePtr;
}

#endif // GS_LAND_TILE_H
