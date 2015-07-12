#ifndef GS_TILE_GROUP_MANAGER_H
#define GS_TILE_GROUP_MANAGER_H

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
        void Add( const gs::TileTexture& texture, const GLuint bufferEnd );
        void SetTextureSize( const GLint newTextureDim );
        void WriteTileGroupsToFile() const;

    public:
        TileGroupManager( const GLint textureDim = 1024 );
    };
}

#endif // GS_TILE_GROUP_MANAGER_H
