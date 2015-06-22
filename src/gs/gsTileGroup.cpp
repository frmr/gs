#include "gsTileGroup.h"

bool gs::TileGroup::AddTile()
{
    //if tile fits in texture
        //add it
        //increase buffer size
        //return true
    //else
        //return false
}

void gs::TileGroup::Draw() const
{
    //bind index array
    //bind texture
    //draw
}

GLuint gs::TileGroup::GetBufferEnd() const
{
    return bufferEnd;
}

void gs::TileGroup::InitBuffers() const
{
    //
}

gs::TileGroup::TileGroup()
    :   bufferBegin( 0 )
{
}

gs::TileGroup::TileGroup( const GLuint bufferBegin )
    :   bufferBegin( bufferBegin )
{
}
