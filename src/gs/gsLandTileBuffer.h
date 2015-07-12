#ifndef GS_LAND_TILE_BUFFER_H
#define GS_LAND_TILE_BUFFER_H

#include "../gl3w/gl3w.h"

#include "gsLandTile.h"
#include "gsTileBuffer.h"

namespace gs
{
    class LandTileBuffer : public gs::TileBuffer
    {
    private:
        GLuint  texCoordVbo;

    public:
        bool Add( gs::LandTile& tile )
        {
            tile.InitBuffers( positionVbo, colorVbo, fogVbo, texCoordVbo );
        }
    };
}

#endif // GS_LAND_TILE_BUFFER_H
