#ifndef GS_TILE_TEXTURE_H
#define GS_TILE_TEXTURE_H

#include <limits>
#include <vector>

#include "gsVec2.h"
#include "gsVertex.h"
#include "gsTile.h"
#include "gsLink.h"

#include "../EasyBMP/EasyBMP.h"

using std::vector;

namespace gs
{
    class TileTexture
    {
    private:
        int width;
        int height;
        gs::Vec2f texCoords;

        BMP file;
    private:
        static gs::Vec2f ComputeRelativePoint( const gs::Vec3f& uAxis, const gs::Vec3f& vAxis, const gs::Vec3f& p );

    public:
        TileTexture( const int id, const vector<gs::VertexPtr>& worldVertices, const vector<gs::Link<gs::Tile>>& links, const gs::Vec3f& centroid );
    };
}
#endif // GS_TILE_TEXTURE_H
