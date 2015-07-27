#include "gsWaterTileBuffer.h"

vector<GLuint> gs::WaterTileBuffer::BuildIndexVector( vector<gs::WaterTilePtr>& waterTiles ) const
{
    vector<GLuint> indexVector;
    for ( auto& tile : waterTiles )
    {
        tile->AddVerticesToIndexVector( indexVector );
    }
    return indexVector;
}

size_t gs::WaterTileBuffer::CountVertices( const vector<gs::WaterTilePtr>& waterTiles ) const
{
    size_t numOfVertices = 0;
    for ( const auto& tile : waterTiles )
    {
        numOfVertices += tile->GetNumOfVertices();
    }
    return numOfVertices;
}

gs::WaterTileBuffer::WaterTileBuffer( vector<gs::WaterTilePtr>& waterTiles, gs::Shader& shader )
    :   TileBuffer( CountVertices( waterTiles ), shader, BuildIndexVector( waterTiles ) )
{
    for ( auto& tile : waterTiles )
    {
        tile->InitBuffers( positionVbo, colorVbo, fogVbo );
    }
}
