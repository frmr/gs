#include "gsGlobe.h"

#include <iostream>

#include "../voronoi/src/voronoi_generator.h"
#include "gsRandomRange.h"
#include "../cck/cck.h"

using std::cerr;
using std::endl;

void gs::Globe::Draw( const gs::Camera& worldCamera ) const
{
    shader.Use();

    glDisable( GL_CULL_FACE );

    //get the inverse model view matrix
    Matrix4 inverseModelViewMatrix = worldCamera.GetViewMatrix();
    inverseModelViewMatrix.invert();

    //pass matrices to shaders
    glUniformMatrix4fv( modelViewMatrixLocation, 1, false, worldCamera.GetViewMatrix().get() ); //TODO: multiply by model matrix if needed
    glUniformMatrix4fv( projectionMatrixLocation, 1, false, worldCamera.GetProjectionMatrix().get() );
    glUniformMatrix4fv( normalMatrixLocation, 1, false, inverseModelViewMatrix.getTranspose() );

    //draw
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexBuffer );
    glDrawElements( GL_TRIANGLES, numOfIndices, GL_UNSIGNED_INT, 0 );
}

cck::Globe gs::Globe::GenerateTerrain() const
{
    cck::Globe terrain( 6370.0, 0 ); //TODO: random seed
    terrain.SetNoiseParameters( 8, 0.75, 0.00015 );
    terrain.AddNode( 8, 57.0, 44.0, -0.3, 0.75, 1200.0 );
    return terrain;
}

void gs::Globe::Update()
{

}

GLuint gs::Globe::CreateVbo( const void* data, const int size, const int components, const string& name )
{
    GLuint vbo;
    glGenBuffers( 1, &vbo );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );

    GLuint location = shader.GetAttribLocation( name );

    glBufferData( GL_ARRAY_BUFFER, size * sizeof(GLfloat), data, GL_STATIC_DRAW );
    glVertexAttribPointer( location, components, GL_FLOAT, GL_FALSE, 0, 0 );
    glEnableVertexAttribArray( location );

    return vbo;
}

GLuint gs::Globe::CreateVbo( const int elements, const int components, const string& name )
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

gs::Globe::Globe()
    :   shader( "test", "data/shaders/test.vert", "data/shaders/test.frag" )
{
    //generate world
    cck::Globe terrain = GenerateTerrain();

    int numOfTiles = 10000;
    tiles.reserve( numOfTiles );

    VoronoiGenerator vg;
    vg.generateTessellation( numOfTiles );

    int vertexCount = 0;
    gs::RandomRange randColor( 0.0f, 255.0f );

    for ( auto& cell : vg.cell_vector )
    {
        vector<gs::Vec3f> cellVertices;
        for ( auto& corner : cell->corners )
        {
            cellVertices.push_back( gs::Vec3f( corner.x, corner.y, corner.z ) );
        }
        tiles.push_back( gs::Tile( vertexCount, cellVertices, terrain, randColor ) );
        vertexCount += cell->corners.size();
    }

    //determine tile neighbours

    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    //copy data to vram
    positionVbo = CreateVbo( vertexCount, 3, "positionVert" );
    colorVbo = CreateVbo( vertexCount, 3, "colorVert");
    texCoordVbo = CreateVbo( vertexCount, 2, "texCoordVert" );
    fogVbo = CreateVbo( vertexCount, 1, "fogVert" );

    vector<GLuint> indexVector;

    for ( auto& tile : tiles )
    {
        tile.InitBuffers( positionVbo, colorVbo, texCoordVbo, fogVbo, indexVector );
    }

    GLuint* indexArray = new GLuint[indexVector.size()];
    for ( unsigned int i = 0; i < indexVector.size(); ++i )
    {
        indexArray[i] = indexVector[i];
    }

    numOfIndices = indexVector.size();

    //indices
    glGenBuffers( 1, &indexBuffer );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexBuffer );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, numOfIndices * sizeof(GLuint), indexArray, GL_STATIC_DRAW );

    delete[] indexArray;

    shader.Link();

    modelViewMatrixLocation = shader.GetUniformLocation( "modelViewMatrix" );
    projectionMatrixLocation = shader.GetUniformLocation( "projectionMatrix" );
    normalMatrixLocation = shader.GetUniformLocation( "normalMatrix" );
}
