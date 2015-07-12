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

bool gs::TileGroup::Add( const gs::TileTexture& tileTexture, const GLuint tileBufferEnd )
{
    double worstFitRatio = std::numeric_limits<double>::max();
//    vector<gs::TileGroup::Rectangle>::iterator worstFitRectangle;

//    for ( auto it = rectangles.begin(); it != rectangles.end(); ++it )
//    {
//        cerr << tileTexture.GetWidth() << " " << tileTexture.GetHeight() << endl;
//        cerr << it->width << " " << it->height << endl;
//        //TODO: check if tile texture can be rotated to fit the rectangle
//        if ( tileTexture.GetWidth() <= it->width && tileTexture.GetHeight() <= it->height )
//        {
//            double fitRatio = (double) tileTexture.GetArea() / (double) it->area;
//            if ( fitRatio < worstFitRatio )
//            {
//                worstFitRatio = fitRatio;
//                worstFitRectangle = it;
//            }
//        }
//    }

    int worstFitIndex = -1;

    for ( int i = 0; i < rectangles.size(); ++i )
    {
        if ( !rectangles[i].used )
        {
            if ( tileTexture.GetWidth() <= rectangles[i].width && tileTexture.GetHeight() <= rectangles[i].height )
            {
                double fitRatio = (double) tileTexture.GetArea() / (double) rectangles[i].area;
                if ( fitRatio < worstFitRatio )
                {
                    worstFitRatio = fitRatio;
                    worstFitIndex = i;
                }
            }
        }

    }

    if ( worstFitRatio > 1.0 )
    {
        rectangles.clear();
        return false;
    }
    else
    {
        //cerr << "Adding " << bufferBegin << " with " << tileTexture.GetWidth() << " " << tileTexture.GetHeight() << " at " << rectangles[worstFitIndex].minCoord.x << " " << rectangles[worstFitIndex].minCoord.y << endl;
        bufferEnd = tileBufferEnd;

        //add tile texture to image
        tileTexture.AddToTileGroupTexture( image, rectangles[worstFitIndex].minCoord );

        //split used rectangle
        vector<gs::TileGroup::Rectangle> newRectangles = rectangles[worstFitIndex].Split( gs::Vec2i( tileTexture.GetWidth(), tileTexture.GetHeight() ) );
        rectangles.insert( rectangles.end(), newRectangles.begin(), newRectangles.end() );
        rectangles[worstFitIndex].used = true;
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

void gs::TileGroup::InitBuffers() const
{
    //
}

void gs::TileGroup::WriteToFile() const
{
    image->WriteToFile( ( "data/textures/procedural/" + std::to_string( bufferBegin ) + ".bmp" ).c_str() );
}

gs::TileGroup::TileGroup()
    :   bufferBegin( 0 )
{
}

gs::TileGroup::TileGroup( const GLuint bufferBegin, const int textureDim )
    :   bufferBegin( bufferBegin )
{
    image = new BMP();
    image->SetSize( textureDim, textureDim );
    rectangles.push_back( gs::TileGroup::Rectangle( gs::Vec2i( 0, 0 ), gs::Vec2i( textureDim, textureDim ) ) );
    cerr << "New tile group at " << bufferBegin << endl;
}

gs::TileGroup::~TileGroup()
{
    //delete texture data
    DeleteLocalTextureData();
}
