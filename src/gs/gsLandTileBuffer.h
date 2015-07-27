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

    private:
        vector<GLuint> BuildIndexVector( vector<gs::LandTilePtr>& landTiles) const;
        size_t CountVertices( const vector<gs::LandTilePtr>& landTiles ) const;

    public:
        LandTileBuffer( vector<gs::LandTilePtr>& landTiles, gs::Shader& shader );
    };
}

#endif // GS_LAND_TILE_BUFFER_H
