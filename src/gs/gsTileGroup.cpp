#include "gsTileGroup.h"

#include <iostream>

using std::cerr;
using std::endl;

vector<gs::TileGroup::Rectangle> gs::TileGroup::Rectangle::Split( const gs::Vec2i& relativeCoord ) const
{
    //TODO: improve splitting algorithm
    vector<gs::TileGroup::Rectangle> newRectangles;

    gs::TileGroup::Rectangle rectangleA( gs::Vec2i( minCoord.x, minCoord.y + relativeCoord.y ), maxCoord );
    gs::TileGroup::Rectangle rectangleB( gs::Vec2i( minCoord.x + relativeCoord.x, minCoord.y ), gs::Vec2i( maxCoord.x, minCoord.y + relativeCoord.y ) );

    if ( rectangleA.area != 0 )
    {
        newRectangles.push_back( rectangleA );
    }
    if ( rectangleB.area != 0 )
    {
        newRectangles.push_back( rectangleB );
    }
    return newRectangles;
}

gs::TileGroup::Rectangle::Rectangle( const gs::Vec2i& minCoord, const gs::Vec2i& maxCoord )
    :   minCoord( minCoord ),
        maxCoord( maxCoord ),
        width( maxCoord.x - minCoord.x ),
        height( maxCoord.y - minCoord.y ),
        area( width * height ),
        used( false )
{
}

//bool gs::TileGroup::Add( const gs::TileTexture& tileTexture, const GLuint tileBufferEnd )
bool gs::TileGroup::Add( const gs::LandTilePtr& landTile )
{
    //add landTile to tiles as static cast to TilePtr

    //TODO: Improve bin packing
    //if tile texture doesn't fit on current shelf
    if ( tileTexture.GetWidth() >= textureDim - shelfCursor.x )
    {
        //create new shelf
        shelfCursor.x = 0;
        shelfCursor.y = shelfTop;
    }

    //if tile texture is taller than current shelf
    if ( tileTexture.GetHeight() > shelfTop - shelfCursor.y )
    {
        //raise the shelf
        shelfTop = shelfCursor.y + tileTexture.GetHeight();
    }

    if ( shelfTop > textureDim )
    {
        return false;
    }
    else
    {
        //bufferEnd = tileBufferEnd;
        bufferEnd = landTile.GetBufferEnd();
        landTile->GetTileTexture().AddToTileGroupTexture( image, shelfCursor );
        shelfCursor.x += tileTexture.GetWidth();


        return true;
    }
}

void gs::TileGroup::DeleteLocalTextureData()
{
    //delete image;
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

void gs::TileGroup::PopulateIndexBuffer()
{
    vector<GLuint> indexVector;
    for ( const auto& tile : tiles )
    {
        tile->
    }
}

void gs::TileGroup::WriteToFile() const
{
    image->WriteToFile( ( "data/textures/procedural/" + std::to_string( bufferBegin ) + ".bmp" ).c_str() );
}

//gs::TileGroup::TileGroup()
//    :   bufferBegin( 0 )
//{
//}

gs::TileGroup::TileGroup( const GLuint bufferBegin, const int textureDim )
    :   textureDim( textureDim ),
        bufferBegin( bufferBegin ),
        bufferEnd( 0 ),
        shelfCursor( 0, 0 ),
        shelfTop( 0 )
{

    glGenBuffers( 1, &indexBuffer );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexBuffer );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, numOfIndices * sizeof(GLuint), indexArray, GL_STATIC_DRAW );

    image = new BMP();
    image->SetSize( textureDim, textureDim );
    //rectangles.push_back( gs::TileGroup::Rectangle( gs::Vec2i( 0, 0 ), gs::Vec2i( textureDim, textureDim ) ) );
    cerr << "New tile group at " << bufferBegin << endl;
}

gs::TileGroup::~TileGroup()
{
    //delete texture data
    DeleteLocalTextureData();
}
