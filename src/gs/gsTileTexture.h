#ifndef GS_TILE_TEXTURE_H
#define GS_TILE_TEXTURE_H

#include <limits>
#include <vector>

#include "gsVec2.h"
#include "gsVertex.h"
#include "gsTile.h"
#include "gsLink.h"
#include "gsRandomRange.h" //TODO: delete

#include "../EasyBMP/EasyBMP.h"

using std::vector;

namespace gs
{
    class TileTexture
    {
    private:
        int         width;
        int         height;
        gs::Vec2f   texCoords;

        BMP*        image;

    public:
        static gs::RandomRange<int> colorGenerator; //TODO: delete

        void    AddToTileGroupTexture( BMP* tileGroupTexture, const gs::Vec2i& coord ) const;
        void    DeleteTextureData();
        int     GetArea() const;
        int     GetHeight() const;
        int     GetWidth() const;



    public:
        TileTexture( const int id, const vector<gs::VertexPtr>& worldVertices, const vector<gs::Link<gs::Tile>>& links, const gs::Vec3f& centroid );
        ~TileTexture();
    };
}
#endif // GS_TILE_TEXTURE_H
