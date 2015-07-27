#ifndef GS_WATER_TILE_H
#define GS_WATER_TILE_H

#include "gsTile.h"
#include "gsVertex.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::vector;

namespace gs
{
    class Tile;

    class WaterTile : public gs::Tile
    {
    private:
        bool ice;

    public:
        void InitBuffers( const GLuint positionVbo, const GLuint colorVbo, const GLuint fogVbo );

    public:
        WaterTile( const vector<shared_ptr<gs::Vertex>>& vertices, const gs::Vec3f& centroid );
    };

    typedef shared_ptr<gs::WaterTile> WaterTilePtr;
}

#endif // GS_WATER_TILE_H
