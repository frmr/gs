#include "gsTileGroupManager.h"
#include "gsLandTile.h"

void gs::TileGroupManager::Add( const LandTilePtr& landTile )
{
    if ( landTileGroups.empty() )
    {
        landTileGroups.push_back( gs::LandTileGroup( 0, textureDim ) );
    }

    //if ( !tileGroups.back().Add( texture, bufferEnd ) )
    if ( !landTileGroups.back().Add( landTile ) )
    {
        landTileGroups.push_back( gs::LandTileGroup( landTileGroups.back().GetBufferEnd() + 1, textureDim ) );
        if ( !landTileGroups.back().Add( landTile ) )
        {
            cerr << "gs::TileGroupManager::Add() in src/gs/TileGroupManager.h: Could not add tile texture to empty tile group. Tile texture is too large." << endl;
        }
    }
}

void gs::TileGroupManager::DrawAll() const
{
    for ( const auto& group : landTileGroups )
    {
        group.Draw();
    }
}

void gs::TileGroupManager::SetTextureSize( const GLint newTextureDim )
{
    textureDim = newTextureDim;
}

void gs::TileGroupManager::WriteTileGroupsToFile() const
{
    for ( const auto& group : landTileGroups )
    {
        group.WriteToFile();
    }
}

gs::TileGroupManager::TileGroupManager( const GLint textureDim )
    :   textureDim( textureDim )
{
}
