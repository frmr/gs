#ifndef GS_TILE_GROUP_H
#define GS_TILE_GROUP_H

#include <vector>

#include "../gl3w/gl3w.h"
#include "../EasyBMP/EasyBMP.h"

#include "gsTile.h"
#include "gsTileTexture.h"

using std::vector;

namespace gs
{
    class TileGroup
    {
    private:
        //texture id
        //pointer to image data (to be deleted eventually)
        const int           textureDim;
        const GLuint        bufferBegin;
        GLuint              bufferEnd;

        GLuint              indexBuffer;

        gs::Vec2i           shelfCursor;
        int                 shelfTop;

        BMP*                image;

        vector<gs::TilePtr> tiles;

    public:
        bool    Add( const gs::TileTexture& tileTexture, const GLuint tileBufferEnd );
        void    DeleteLocalTextureData();
        void    Draw() const;
        GLuint  GetBufferEnd() const;
        void    InitBuffers() const;
        void    PopulateIndexBuffer();
        void    WriteToFile() const;

    public:
        //TileGroup();
        TileGroup( const GLuint bufferBegin, const int textureDim );
        ~TileGroup();
    };
}

#endif
