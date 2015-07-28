#ifndef GS_TILE_GROUP_H
#define GS_TILE_GROUP_H

#include <vector>

#include "../gl3w/gl3w.h"
#include "../EasyBMP/EasyBMP.h"

#include "gsTile.h"

using std::vector;

namespace gs
{
    class TileGroup
    {
    protected:
        //texture id
        //pointer to image data (to be deleted eventually)
        const GLuint        bufferBegin;
        GLuint              bufferEnd;

    public:
        virtual void    Draw() const = 0;
        GLuint          GetBufferBegin() const;
        GLuint          GetBufferEnd() const;

    public:
        //TileGroup();
        TileGroup( const GLuint bufferBegin );
    };
}

#endif
