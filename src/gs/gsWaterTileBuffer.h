#ifndef GS_WATER_TILE_BUFFER_H
#define GS_WATER_TILE_BUFFER_H

#include "../gl3w/gl3w.h"

#include "gsWaterTile.h"
#include "gsTileBuffer.h"

namespace gs
{
    class WaterTileBuffer : public gs::TileBuffer
    {
    public:
        bool Add( gs::WaterTile& tile )
        {
            tile.InitBuffers( positionVbo, colorVbo, fogVbo );
        }
    };
}
#endif // GS_WATER_TILE_BUFFER_H
