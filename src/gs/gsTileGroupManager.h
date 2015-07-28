#ifndef GS_TILE_GROUP_MANAGER_H
#define GS_TILE_GROUP_MANAGER_H

#include "gsLandTile.h"
#include "gsLandTileGroup.h"
#include "gsTileTexture.h"
#include "gsWaterTile.h"
#include "gsWaterTileGroup.h"

#include <iostream>
#include <vector>

using std::cerr;
using std::endl;
using std::vector;

namespace gs
{
    class TileGroupManager
    {
    private:
        GLint                       textureDim;
        vector<gs::LandTileGroup>   landTileGroups;
        gs::WaterTileGroup          waterTileGroup;

    public:
        void Add( const gs::LandTilePtr& landTile );
        void Add( const gs::WaterTilePtr& waterTile );
        void DrawAll() const;
        void DrawLandTileGroups() const;
        void DrawWaterTileGroup() const;
        void SetTextureSize( const GLint newTextureDim );
        void WriteTileGroupsToFile() const;

    public:
        TileGroupManager( const GLint textureDim = 1024 );
    };
}

#endif // GS_TILE_GROUP_MANAGER_H
