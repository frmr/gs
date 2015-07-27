#include "gsTile.h"

#include "gsLandTile.h"
#include "gsWaterTile.h"

#include <iostream>
#include <limits>

using std::cerr;
using std::endl;

int gs::Tile::idCounter = 0;

void gs::Tile::AddVerticesToIndexVector( vector<GLuint>& indexVector )
{
    indexVector.reserve( indexVector.size() + 3 * vertices.size() );

    for ( unsigned int i = 1; i < vertices.size() - 1; ++i )
    {
        indexVector.push_back( bufferOffset );
        indexVector.push_back( bufferOffset + i );
        indexVector.push_back( bufferOffset + i + 1 );
    }
}

void gs::Tile::InitColorBuffer( const GLuint colorVbo ) //TODO: const
{
    GLfloat* colorArray = new GLfloat[3*vertices.size()];
    for ( unsigned int i = 0; i < vertices.size(); ++i )
    {
        colorArray[3*i] = color.x;
        colorArray[3*i+1] = color.y;
        colorArray[3*i+2] = color.z;
    }
    glBindBuffer( GL_ARRAY_BUFFER, colorVbo );
    glBufferSubData( GL_ARRAY_BUFFER, 3 * bufferOffset * sizeof(GLfloat), 3 * vertices.size() * sizeof(GLfloat), colorArray );
    delete[] colorArray;
}

void gs::Tile::InitFogBuffer( const GLuint fogVbo ) //TODO: const
{
    GLfloat* fogArray = new GLfloat[vertices.size()];
    for ( unsigned int i = 0; i < vertices.size(); ++i )
    {
        fogArray[i] = 0.0f;
    }
    glBindBuffer( GL_ARRAY_BUFFER, fogVbo );
    glBufferSubData( GL_ARRAY_BUFFER, bufferOffset * sizeof(GLfloat), vertices.size() * sizeof(GLfloat), fogArray );
    delete[] fogArray;
}

void gs::Tile::InitPositionBuffer( const GLuint positionVbo ) //TODO: const
{
    GLfloat* positionArray = new GLfloat[3*vertices.size()];
    for ( unsigned int i = 0; i < vertices.size(); ++i )
    {
        positionArray[3*i] = (float) vertices[i]->position.x;
        positionArray[3*i+1] = (float) vertices[i]->position.y;
        positionArray[3*i+2] = (float) vertices[i]->position.z;
    }
    glBindBuffer( GL_ARRAY_BUFFER, positionVbo );
    glBufferSubData( GL_ARRAY_BUFFER, 3 * bufferOffset * sizeof(GLfloat), 3 * vertices.size() * sizeof(GLfloat), positionArray );
    delete[] positionArray;
}

bool gs::Tile::AddLink( const gs::Link<gs::Tile>& link )
{
    if ( allLinks.size() == vertices.size() )
    {
        return false;
    }
    allLinks.push_back( link );
    return true;
}

bool gs::Tile::AddLink( const gs::Link<gs::LandTile>& link )
{
    if ( landLinks.size() == vertices.size() )
    {
        return false;
    }
    landLinks.push_back( link );
    return true;
}

bool gs::Tile::AddLink( const gs::Link<gs::WaterTile>& link )
{
    if ( waterLinks.size() == vertices.size() )
    {
        return false;
    }
    waterLinks.push_back( link );
    return true;
}

GLuint  gs::Tile::GetBufferEnd() const
{
    return bufferOffset + vertices.size() - 1;
}

GLuint  gs::Tile::GetBufferOffset() const
{
    return bufferOffset;
}

double gs::Tile::GetHeight() const
{
    return height;
}

gs::Tile::Type gs::Tile::GetSurface() const
{
    return surface;
}

void gs::Tile::SetBufferOffset( GLuint& vertexCount )
{
    bufferOffset = vertexCount;
    vertexCount += vertices.size();
}

gs::Tile::Tile( const Type surface, const vector<shared_ptr<gs::Vertex>>& vertices, const gs::Vec3f& centroid, const double height )
    :   id( idCounter++ ),
        surface( surface ),
        centroid( centroid ),
        height( height ),
        vertices( vertices ),
        fog( false )
{
}

//gs::Tile::Tile( const int bufferOffset, const vector<shared_ptr<gs::Vertex>>& vertices )
//    :   surface( gs::Tile::Type::UNASSIGNED ),
//        height( 0.0 ),
//        bufferOffset( bufferOffset ),
//        vertices( vertices ),
//        fog( false )
//{
//}

gs::Tile::~Tile()
{
}
