#ifndef GS_WATER_TILE_BUFFER_H
#define GS_WATER_TILE_BUFFER_H

#include "../gl3w/gl3w.h"

#include <vector>

#include "gsWaterTile.h"
#include "gsTileBuffer.h"

using std::vector;

namespace gs
{
    class WaterTileBuffer : public gs::TileBuffer
    {
    public:
        bool Add( gs::WaterTile& tile, vector<GLuint>& indexVector )
        {
            tile.InitBuffers( positionVbo, colorVbo, fogVbo, indexVector );
        }
    };
}
#endif // GS_WATER_TILE_BUFFER_H
