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
        class Rectangle
        {
        public:
            gs::Vec2i   minCoord;
            gs::Vec2i   maxCoord;
            int         width;
            int         height;
            int         area;
            bool        used;

        public:
            //splits always occur from the same corner
            vector<Rectangle> Split( const gs::Vec2i& coord ) const;

        public:
            Rectangle( const gs::Vec2i& minCoord, const gs::Vec2i& maxCoord );
        };

    private:
        //texture id
        //pointer to image data (to be deleted eventually)
        const int           textureDim;
        const GLuint        bufferBegin;
        GLuint              bufferEnd;

        gs::Vec2i           shelfCursor;
        int                 shelfTop;

        BMP*                image;

        //vector<Rectangle>   rectangles;

    public:
        bool    Add( const gs::TileTexture& tileTexture, const GLuint tileBufferEnd );
        void    DeleteLocalTextureData();
        void    Draw() const;
        GLuint  GetBufferEnd() const;
        void    InitBuffers() const;
        void    WriteToFile() const;

    public:
        //TileGroup();
        TileGroup( const GLuint bufferBegin, const int textureDim );
        ~TileGroup();
    };
}

#endif
