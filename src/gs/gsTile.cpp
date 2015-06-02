#include "gsTile.h"

#include <iostream>

using std::cerr;
using std::endl;

void gs::Tile::InitBuffers( const GLuint positionVbo, const GLuint colorVbo, const GLuint texCoordVbo, const GLuint fogVbo, vector<GLuint>& indexVector ) const
{
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
    //cerr << "----" << endl;
    for ( unsigned int i = 1; i < vertices.size() - 1; ++i )
    {
        indexVector.push_back( bufferOffset );
        indexVector.push_back( bufferOffset + i );
        indexVector.push_back( bufferOffset + i + 1 );
        //cerr << bufferOffset << " " << bufferOffset + i << " " << bufferOffset + i + 1 << endl;
    }
}

gs::Tile::Tile( const int bufferOffset, const vector<gs::Vec3f>& vertices, gs::RandomRange& randColor )
    :   bufferOffset( bufferOffset ),
        vertices( vertices ),
        color( randColor.Sample()/255.0f, randColor.Sample()/255.0f, randColor.Sample()/255.0f ),
        fog( false )
{
}
