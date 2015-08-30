#include "gsLandTileGroup.h"
#include <iostream>

using std::cerr;
using std::endl;

//GLuint gs::LandTileGroup::GenerateTextureId()
//{
//    GLuint id;
//    glGenTextures( 1, &id );
//    return id;
//}

bool gs::LandTileGroup::Add( const gs::LandTilePtr& landTile )
{
    //TODO: Improve bin packing
    //if tile texture doesn't fit on current shelf
    if ( landTile->GetTexture()->GetWidth() >= textureSize - shelfCursor.x )
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

    if ( shelfTop > textureSize )
    {
        return false;
    }
    else
    {
        bufferEnd = landTile->GetIndexBufferEnd();
        landTile->AddToTileGroupTexture( texture, shelfCursor, textureSize );
        shelfCursor.x += landTile->GetTexture()->GetWidth();
        return true;
    }
}

void gs::LandTileGroup::DeleteLocalTextureData()
{
    texture.reset();
}

void gs::LandTileGroup::Draw() const
{
    glBindTexture( GL_TEXTURE_2D, textureId );
    //glUniform1i( 10, textureId );
    glDrawElements( GL_TRIANGLES, ( bufferEnd - bufferBegin ) + 1, GL_UNSIGNED_INT, (void*) ( bufferBegin * sizeof(GLuint) )  ); //TODO: fix this memory leak (might be drivers?)
}

void gs::LandTileGroup::PushTexture()
{
    textureId = texture->Push();
    //glBindTexture( textureId );
    //glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    //glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
}

void gs::LandTileGroup::WriteToFile() const
{
    texture->WriteToFile( ( "data/textures/procedural/" + std::to_string( bufferBegin ) + ".bmp" ) );
}

gs::LandTileGroup::LandTileGroup( const GLuint bufferBegin, const int textureSize )
    :   TileGroup( bufferBegin ),
        textureSize( textureSize ),
        textureId( 0 ),
        shelfCursor( 0, 0 ),
        shelfTop( 0 )
{
    texture = std::make_shared<gs::Texture>( textureSize, textureSize );
}

gs::LandTileGroup::~LandTileGroup()
{
    DeleteLocalTextureData();
    //TODO: delete GL texture data
}
