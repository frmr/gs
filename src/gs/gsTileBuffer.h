#ifndef GS_TILE_BUFFER_H
#define GS_TILE_BUFFER_H

#include "../gl3w/gl3w.h"

namespace gs
{
    class TileBuffer
    {
    private:
        GLuint  positionVbo;
        GLuint  colorVbo;
        GLuint  fogVbo;
        GLuint  indexBuffer;

    public:

        bool Update( const GLuint tileIndex, const gs::Vec3f& newColor, const bool newFog )
        {

        }

    public:
        TileBuffer( const int size )
        {

        }
    };
}
#endif // GS_TILE_BUFFER_H
