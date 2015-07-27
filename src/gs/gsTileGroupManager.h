#ifndef GS_TILE_GROUP_MANAGER_H
#define GS_TILE_GROUP_MANAGER_H

#include "gsLandTile.h"
#include "gsTileGroup.h"
#include "gsTileTexture.h"

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
        GLint                   textureDim;
        vector<gs::TileGroup>   tileGroups;

    public:
        void Add( const gs::LandTilePtr& landTile );
        void DrawAll() const;
        void PopulateIndexBuffers();
        void SetTextureSize( const GLint newTextureDim );
        void WriteTileGroupsToFile() const;

    public:
        TileGroupManager( const GLint textureDim = 1024 );
    };
}

#endif // GS_TILE_GROUP_MANAGER_H
