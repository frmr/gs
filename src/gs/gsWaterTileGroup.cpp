#include "gsWaterTileGroup.h"

bool gs::WaterTileGroup::Add( const gs::WaterTilePtr& waterTile )
{
    bufferEnd = waterTile->GetIndexBufferEnd();
    return true;
}

void gs::WaterTileGroup::Draw() const
{
    glDrawElements( GL_TRIANGLES, ( bufferEnd - bufferBegin ) + 1, GL_UNSIGNED_INT, (void*)( bufferBegin * sizeof(GLuint) ) );
}

gs::WaterTileGroup::WaterTileGroup( const GLuint bufferBegin )
    :   TileGroup( bufferBegin )
{
}
