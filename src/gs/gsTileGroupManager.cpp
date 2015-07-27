#include "gsTileGroupManager.h"
#include "gsLandTile.h"

//void gs::TileGroupManager::Add( const gs::TileTexture& texture, const GLuint bufferEnd )
void gs::TileGroupManager::Add( const LandTilePtr& landTile )
{
    if ( tileGroups.empty() )
    {
        tileGroups.push_back( gs::TileGroup( 0, textureDim ) );
    }

    //if ( !tileGroups.back().Add( texture, bufferEnd ) )
    if ( !tileGroups.back().Add( landTile ) )
    {
        tileGroups.push_back( gs::TileGroup( tileGroups.back().GetBufferEnd() + 1, textureDim ) );
        //if ( !tileGroups.back().Add( texture, bufferEnd ) )
        if ( !tileGroups.back().Add( landTile ) )
        {
            cerr << "gs::TileGroupManager::Add() in src/gs/TileGroupManager.h: Could not add tile texture to empty tile group. Tile texture is too large." << endl;
        }
    }
}

void gs::TileGroupManager::DrawAll() const
{
    for ( const auto& group : tileGroups )
    {
        group.Draw();
    }
}

void gs::TileGroupManager::PopulateIndexBuffers()
{
    for ( auto& group : tileGroups )
    {
        group.PopulateIndexBuffer();
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
