#ifndef GS_LAND_TILE_GROUP_H
#define GS_LAND_TILE_GROUP_H

#include <vector>

#include "../gl3w/gl3w.h"
#include "../EasyBMP/EasyBMP.h"

#include "gsLandTile.h"
#include "gsTile.h"
#include "gsTileGroup.h"
#include "gsTexture.h"

using std::vector;

namespace gs
{
    class LandTileGroup : public gs::TileGroup
    {
    private:
        const int           textureDim;
        const GLuint        textureId;
        gs::Vec2i           shelfCursor;
        int                 shelfTop;
        //BMP*                image;
        gs::Texture*        texture;

    private:
        static GLuint   GenerateTextureId();

    public:
        bool            Add( const gs::LandTilePtr& landTile );
        void            DeleteLocalTextureData();
        void            Draw() const;
        void            LoadTexture() const;
        void            WriteToFile() const;

    public:
        LandTileGroup( const GLuint bufferBegin, const int textureDim );
        ~LandTileGroup();
    };
}

#endif
