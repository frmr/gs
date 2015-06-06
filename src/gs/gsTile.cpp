#include "gsTile.h"

#include <iostream>
#include <limits>

using std::cerr;
using std::endl;

void gs::Tile::SetColor() //delete
{
    color = gs::Vec3f( 1.0f, 1.0f, 0.0f );
    visited = true;
    for ( auto link : links )
    {
        if ( !link.target->visited )
        {
            link.target->SetColor();
        }

    }
}

void gs::Tile::InitBuffers( const GLuint positionVbo, const GLuint colorVbo, const GLuint texCoordVbo, const GLuint fogVbo, vector<GLuint>& indexVector ) //const
{
    if ( bufferOffset == 0 )
    {
        color = gs::Vec3f( 1.0f, 0.0f, 0.0f );
        for ( auto link : links )
        {
            if ( !link.target->visited )
            {
                link.target->SetColor();
            }

        }
    }

    //load position data
    GLfloat* positionArray = new GLfloat[3*vertices.size()];
    for ( unsigned int i = 0; i < vertices.size(); ++i )
    {
        positionArray[3*i] = vertices[i].x;
        positionArray[3*i+1] = vertices[i].y;
        positionArray[3*i+2] = vertices[i].z;
    }
    glBindBuffer( GL_ARRAY_BUFFER, positionVbo );
    glBufferSubData( GL_ARRAY_BUFFER, 3 * bufferOffset * sizeof(GLfloat), 3 * vertices.size() * sizeof(GLfloat), positionArray );
    delete[] positionArray;

    //load color data
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

    //load texCoord data
    GLfloat* texCoordArray = new GLfloat[2*vertices.size()];
    for ( unsigned int i = 0; i < vertices.size(); ++i )
    {
        texCoordArray[2*i] = 0.0f;
        texCoordArray[2*i+1] = 0.0f;
    }
    glBindBuffer( GL_ARRAY_BUFFER, texCoordVbo );
    glBufferSubData( GL_ARRAY_BUFFER, 2 * bufferOffset * sizeof(GLfloat), 2 * vertices.size() * sizeof(GLfloat), texCoordArray );
    delete[] texCoordArray;

    //load fog data
    GLfloat* fogArray = new GLfloat[vertices.size()];
    for ( unsigned int i = 0; i < vertices.size(); ++i )
    {
        fogArray[i] = 0.0f;
    }
    glBindBuffer( GL_ARRAY_BUFFER, fogVbo );
    glBufferSubData( GL_ARRAY_BUFFER, bufferOffset * sizeof(GLfloat), vertices.size() * sizeof(GLfloat), fogArray );
    delete[] fogArray;

    //append indices to vector
    //indexVector.reserve( indexVector.size() + 3 * vertices.size() );
    for ( unsigned int i = 1; i < vertices.size() - 1; ++i )
    {
        indexVector.push_back( bufferOffset );
        indexVector.push_back( bufferOffset + i );
        indexVector.push_back( bufferOffset + i + 1 );
    }
}

bool gs::Tile::AddLink( const gs::Link& link )
{
    if ( links.size() == vertices.size() )
    {
        return false;
    }
    links.push_back( link );
    return true;
}

gs::Tile::Tile( const int bufferOffset, const vector<gs::Vec3f>& vertices, const cck::Globe& terrain )
    :   bufferOffset( bufferOffset ),
        vertices( vertices ),
        //color( randColor.Sample()/255.0f, randColor.Sample()/255.0f, randColor.Sample()/255.0f ),
        fog( false )
{
    cck::Vec3 center;
    for ( const auto& vertex : vertices )
    {
        center += cck::Vec3( vertex.z, vertex.x, vertex.y );
    }
    center /= vertices.size();

    double height = std::numeric_limits<double>::min();
    int id;
    terrain.SampleData( center.ToGeographic(), height, id );
    color = ( height > 0.0001 ) ? gs::Vec3f( 0.0f, 1.0f, 0.0f ) : gs::Vec3f( 0.0f, 0.0f, 1.0f );
}
