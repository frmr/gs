#include "gsGlobe.h"

#include <iostream>

#include "../voronoi/src/voronoi_generator.h"

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

    //glBindBuffer( GL_ARRAY_BUFFER, positionVbo );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexBuffer );

    //glDrawElements( GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_BYTE, 0 );
    glDrawElements( GL_TRIANGLES, numOfIndices, GL_UNSIGNED_INT, 0 );

    //glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    //glDrawArrays(GL_LINE_LOOP, 0, 4);
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
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<float> dist(0.0f, 255.0f);

    int numOfTiles = 100;
    tiles.reserve( numOfTiles );

    VoronoiGenerator vg;
    vg.generateTessellation( numOfTiles );

    int vertexCount = 0;

    for ( auto& cell : vg.cell_vector )
    {
        vector<gs::Vec3f> cellVertices;
        for ( auto& corner : cell->corners )
        {
            cellVertices.push_back( gs::Vec3f( corner.x, corner.y, corner.z ) );
        }
        tiles.push_back( gs::Tile( vertexCount, cellVertices, dist, mt ) );
        vertexCount += cell->corners.size();
    }

    //determine tile neighbours
    //generate world

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

//    /* The four vericies of a tetrahedron */
//    const GLfloat tetrahedron[12] = { 1.0,  1.0,  1.0, -1.0, -1.0, 1.0, -1.0,  1.0, -1.0, 1.0, -1.0, -1.0 };
//
//    const GLfloat colors[4][3] = {
//    {  1.0,  0.0,  0.0  }, /* Red */
//    {  0.0,  1.0,  0.0  }, /* Green */
//    {  0.0,  0.0,  1.0  }, /* Blue */
//    {  1.0,  1.0,  1.0  } }; /* White */
//
//    const GLfloat texCoords[4][2] = {
//    { 0.0, 0.0 },
//    { 0.0, 0.0 },
//    { 0.0, 0.0 },
//    { 0.0, 0.0 } };
//
//    const GLfloat fog[4] = { 1.0, 1.0, 0.0, 0.0 };
//
//    const GLubyte indices[6] = { 0, 1, 2, 3, 0, 1 };
//    //const GLubyte indices[4] = { 0, 1, 2, 3 };
//

//
//    positionVbo = CreateVbo( tetrahedron, 12, 3, "positionVert" );
//    colorVbo = CreateVbo( colors, 12, 3, "colorVert" );
//    texCoordVbo = CreateVbo( texCoords, 8, 2, "texCoordVert" );
//    fogVbo = CreateVbo( fog, 4, 1, "fogVert" );


    //indices
    glGenBuffers( 1, &indexBuffer );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexBuffer );

    //glBufferData( GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLubyte), indices, GL_STATIC_DRAW );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, numOfIndices * sizeof(GLuint), indexArray, GL_STATIC_DRAW );

    delete[] indexArray;

    shader.Link();

    modelViewMatrixLocation = shader.GetUniformLocation( "modelViewMatrix" );
    projectionMatrixLocation = shader.GetUniformLocation( "projectionMatrix" );
    normalMatrixLocation = shader.GetUniformLocation( "normalMatrix" );

    glBindBuffer( GL_ARRAY_BUFFER, positionVbo );
    GLfloat* data =  new GLfloat[100];
    glGetBufferSubData( GL_ARRAY_BUFFER, 0, 100, data);
    for ( int i = 0; i < 100; ++i )
    {
        //cerr << data[i] << endl;
    }


}
