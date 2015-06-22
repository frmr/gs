#ifndef GS_TILE_GROUP_MANAGER_H
#define GS_TILE_GROUP_MANAGER_H

#include "gsTileGroup.h"
#include "gsTileTexture.h"

#include <vector>

using std::vector;

namespace gs
{
    class TileGroupManager
    {
    private:
        vector<gs::TileGroup> tileGroups;

    public:
        void Add( const gs::TileTexture& texture, const GLuint bufferEnd )
        {
            if ( !tileGroup.end()->Add( texture, bufferEnd ) )
            {
                tileGroups.push_back( gs::TileGroup( tileGroups.end()->GetBufferEnd() + 1 ) );
                tileGroup.end()->Add( texture, bufferEnd );
            }
        }

    public:
        TileGroupManager()
        {
            tileGroups.push_back( gs::TileGroup() );
        }
    };
}

#endif // GS_TILE_GROUP_MANAGER_H
