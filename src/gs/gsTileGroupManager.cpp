#include "gsTileGroupManager.h"

void gs::TileGroupManager::Add( const gs::TileTexture& texture, const GLuint bufferEnd )
{
    if ( tileGroups.empty() )
    {
        tileGroups.push_back( gs::TileGroup( 0, textureDim ) );
    }

    if ( !tileGroups.back().Add( texture, bufferEnd ) )
    {
        tileGroups.push_back( gs::TileGroup( tileGroups.back().GetBufferEnd() + 1, textureDim ) );
        if ( !tileGroups.back().Add( texture, bufferEnd ) )
        {
            cerr << "gs::TileGroupManager::Add() in src/gs/TileGroupManager.h: Could not add tile texture to empty tile group. Tile texture is too large." << endl;
        }
    }
}

void gs::TileGroupManager::SetTextureSize( const GLint newTextureDim )
{
    textureDim = newTextureDim;
}

void gs::TileGroupManager::WriteTileGroupsToFile() const
{
    for ( const auto& tg : tileGroups )
    {
        tg.WriteToFile();
    }
}

gs::TileGroupManager::TileGroupManager( const GLint textureDim )
    :   textureDim( textureDim )
{
}
