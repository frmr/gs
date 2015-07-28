#include "gsWaterTileGroup.h"

bool gs::WaterTileGroup::Add( const gs::WaterTilePtr& waterTile )
{
    bufferEnd = waterTile->GetIndexBufferEnd();
    return true;
}

void gs::WaterTileGroup::Draw() const
{
    glDrawElements( GL_TRIANGLES, bufferEnd, GL_UNSIGNED_INT, (void*)( bufferBegin ) );
}

gs::WaterTileGroup::WaterTileGroup( const GLuint bufferBegin )
    :   TileGroup( bufferBegin )
{
}
