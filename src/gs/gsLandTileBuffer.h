#ifndef GS_LAND_TILE_BUFFER_H
#define GS_LAND_TILE_BUFFER_H

#include "../gl3w/gl3w.h"

#include <vector>

#include "gsLandTile.h"
#include "gsShader.h"
#include "gsTileBuffer.h"

using std::vector;

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

    public:
        LandTileBuffer( const GLuint size, gs::Shader& shader )
            :   TileBuffer( size, shader )
        {

        }
    };
}

#endif // GS_LAND_TILE_BUFFER_H
