#include "gsLandTileGroup.h"
#include <iostream>

using std::cerr;
using std::endl;

GLuint gs::LandTileGroup::GenerateTextureId()
{
    GLuint id;
    glGenTextures( 1, &id );
    return id;
}

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
        //landTile->GetTexture()->AddToTileGroupTexture( image, shelfCursor );
        landTile->AddToTileGroupTexture( texture, shelfCursor );
        shelfCursor.x += landTile->GetTexture()->GetWidth();
        return true;
    }
}

void gs::LandTileGroup::DeleteLocalTextureData()
{
    //delete texture; //TODO: uncomment
}

void gs::LandTileGroup::Draw() const
{
    //bind texture
    glDrawElements( GL_TRIANGLES, ( bufferEnd - bufferBegin ) + 1, GL_UNSIGNED_INT, (void*) ( bufferBegin * sizeof(GLuint) )  );
}

void gs::LandTileGroup::LoadTexture() const
{
    glBindTexture( GL_TEXTURE_2D, textureId );
}

void gs::LandTileGroup::WriteToFile() const
{
    //texture->WriteToFile( ( "data/textures/procedural/" + std::to_string( bufferBegin ) + ".bmp" ).c_str() );
    BMP image;
    image.SetSize( textureDim, textureDim );
    for ( int x = 0; x < textureDim; ++x )
    {
        for ( int y = 0; y < textureDim; ++y )
        {
            image(x,y)->Red = (int) texture->GetRed( x, y );
            image(x,y)->Green = (int) texture->GetGreen( x, y );
            image(x,y)->Blue = (int) texture->GetBlue( x, y );
        }
    }
    image.WriteToFile( ( "data/textures/procedural/" + std::to_string( bufferBegin ) + ".bmp" ).c_str() );
}

gs::LandTileGroup::LandTileGroup( const GLuint bufferBegin, const int textureDim )
    :   TileGroup( bufferBegin ),
        textureDim( textureDim ),
        textureId( GenerateTextureId() ),
        shelfCursor( 0, 0 ),
        shelfTop( 0 )
{
    texture = new gs::Texture( textureDim, textureDim );
}

gs::LandTileGroup::~LandTileGroup()
{
    DeleteLocalTextureData();
}
