#ifndef GS_WATER_TILE_BUFFER_H
#define GS_WATER_TILE_BUFFER_H

#include "../gl3w/gl3w.h"

#include <vector>

#include "gsWaterTile.h"
#include "gsTileBuffer.h"

using std::vector;

namespace gs
{
    class WaterTileBuffer : public gs::TileBuffer
    {
    private:
        vector<GLuint> BuildIndexVector(vector<gs::WaterTilePtr>& waterTiles) const;
        size_t CountVertices(const vector<gs::WaterTilePtr>& waterTiles) const;

    public:
        WaterTileBuffer(vector<gs::WaterTilePtr>& waterTiles, gs::Shader& shader);
    };
}

#endif // GS_WATER_TILE_BUFFER_H
