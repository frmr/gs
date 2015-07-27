#include "gsTileBuffer.h"

GLuint gs::TileBuffer::CreateVbo( const int elements, const int components, gs::Shader& shader, const string& name )
{
    GLuint vbo;
    glGenBuffers( 1, &vbo );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );

    GLuint location = shader.GetAttribLocation( name );

    glBufferData( GL_ARRAY_BUFFER, elements * components * sizeof(GLfloat), NULL, GL_STATIC_DRAW );
    glVertexAttribPointer( location, components, GL_FLOAT, GL_FALSE, 0, 0 );
    glEnableVertexAttribArray( location );

    return vbo;
}

void gs::TileBuffer::Bind()
{
    glBindVertexArray( vao );
}

bool gs::TileBuffer::Update( const GLuint tileIndex, const gs::Vec3f& newColor, const bool newFog )
{

}

gs::TileBuffer::TileBuffer( const int size, gs::Shader& shader )
{
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    positionVbo = CreateVbo( numOfVertices, 3, "positionVert" );
    colorVbo = CreateVbo( numOfVertices, 3, "colorVert");
    texCoordVbo = CreateVbo( numOfVertices, 2, "texCoordVert" );
    fogVbo = CreateVbo( numOfVertices, 1, "fogVert" );
}
