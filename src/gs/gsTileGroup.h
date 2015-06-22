#ifndef GS_TILE_GROUP_H
#define GS_TILE_GROUP_H

#include <GL/gl3w.h>

#include "gsTile.h"

namespace gs
{
    class TileGroup
    {
    private:
        //texture id
        //pointer to image data (to be deleted eventually)
        const GLuint bufferBegin;
        GLuint bufferEnd;

    public:
        bool    AddTile();
        void    Draw() const;
        GLuint  GetBufferEnd() const;
        void    InitBuffers() const;

    public:
        TileGroup();
        TileGroup( const GLuint bufferBegin );
        ~TileGroup();
    };
}

#endif
