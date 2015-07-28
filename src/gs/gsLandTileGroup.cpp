#include "gsLandTileGroup.h"
#include <iostream>

using std::cerr;
using std::endl;

bool gs::LandTileGroup::Add( const gs::LandTilePtr& landTile )
{
    //TODO: Improve bin packing
    //if tile texture doesn't fit on current shelf
    if ( landTile->GetTexture()->GetWidth() >= textureDim - shelfCursor.x )
    {
        //create new shelf
        shelfCursor.x = 0;
        shelfCursor.y = shelfTop;
    }

    //if tile texture is taller than current shelf
    if ( landTile->GetTexture()->GetHeight() > shelfTop - shelfCursor.y )
    {
        //raise the shelf
        shelfTop = shelfCursor.y + landTile->GetTexture()->GetHeight();
    }

    if ( shelfTop > textureDim )
    {
        return false;
    }
    else
    {
        bufferEnd = landTile->GetIndexBufferEnd();
        landTile->GetTexture()->AddToTileGroupTexture( image, shelfCursor );
        shelfCursor.x += landTile->GetTexture()->GetWidth();
        return true;
    }
}

void gs::LandTileGroup::DeleteLocalTextureData()
{
    //delete image;
}

void gs::LandTileGroup::Draw() const
{
    //bind texture
    glDrawElements( GL_TRIANGLES, bufferEnd - bufferBegin + 1, GL_UNSIGNED_INT, (void*) ( bufferBegin * sizeof(GLuint) )  );
    //glDrawElements( GL_TRIANGLES, bufferEnd - bufferBegin + 1, GL_UNSIGNED_INT, (void*) ( bufferBegin )  );
}

void gs::LandTileGroup::WriteToFile() const
{
    image->WriteToFile( ( "data/textures/procedural/" + std::to_string( bufferBegin ) + ".bmp" ).c_str() );
}

gs::LandTileGroup::LandTileGroup( const GLuint bufferBegin, const int textureDim )
    :   TileGroup( bufferBegin ),
        textureDim( textureDim ),
        shelfCursor( 0, 0 ),
        shelfTop( 0 )
{
    image = new BMP();
    image->SetSize( textureDim, textureDim );
}

gs::LandTileGroup::~LandTileGroup()
{
    DeleteLocalTextureData();
}
