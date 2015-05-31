#ifndef GS_TILE_H
#define GS_TILE_H

#include <GL/gl3w.h>

namespace gs
{
    class Tile
    {
    private:
        GLintptr    bufferOffset;
        gs::Vec3f   color;
    };
}

#endif // GS_TILE_H
