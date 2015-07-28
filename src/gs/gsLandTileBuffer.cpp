#include "gsLandTileBuffer.h"

#include <iostream>

using std::cerr;
using std::endl;

vector<GLuint> gs::LandTileBuffer::BuildIndexVector( vector<gs::LandTilePtr>& landTiles ) const
{
    vector<GLuint> indexVector;
    for ( auto& tile : landTiles )
    {
        tile->AddVerticesToIndexVector( indexVector );
    }
    return indexVector;
}

size_t gs::LandTileBuffer::CountVertices( const vector<gs::LandTilePtr>& landTiles ) const
{
    size_t numOfVertices = 0;
    for ( const auto& tile : landTiles )
    {
        numOfVertices += tile->GetNumOfVertices();
    }
    return numOfVertices;
}

gs::LandTileBuffer::LandTileBuffer( vector<gs::LandTilePtr>& landTiles, gs::Shader& shader )
    :   TileBuffer( CountVertices( landTiles ), shader, BuildIndexVector( landTiles ) )
{
    texCoordVbo = CreateVbo( CountVertices( landTiles ), 2, shader, "texCoordVert" );
    for ( auto& tile : landTiles )
    {
        tile->InitBuffers( positionVbo, colorVbo, fogVbo, texCoordVbo );
    }
}