#ifndef GS_WATER_TILE_GROUP_H
#define GS_WATER_TILE_GROUP_H

#include "../gl3w/gl3w.h"

#include "gsWaterTile.h"
#include "gsTile.h"
#include "gsTileGroup.h"

namespace gs
{
    class WaterTileGroup : public gs::TileGroup
    {
    public:
        bool    Add( const gs::WaterTilePtr& waterTile );
        void    Draw() const;

    public:
        WaterTileGroup( const GLuint bufferBegin = 0 );
    };
}

#endif
