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
        WaterTile( const int bufferOffset, const vector<shared_ptr<gs::Vertex>>& vertices );
    };
}

#endif // GS_WATER_TILE_H
