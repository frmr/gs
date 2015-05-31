#include "gsGlobe.h"

#include <iostream>

#include "../voronoi/src/voronoi_generator.h"

using std::cerr;
using std::endl;

void gs::Globe::Draw( const gs::Camera& worldCamera ) const
{
    shader.Use();

    //get the inverse model view matrix
    Matrix4 inverseModelViewMatrix = worldCamera.GetViewMatrix();
    inverseModelViewMatrix.invert();

    //pass matrices to shaders
    glUniformMatrix4fv( modelViewMatrixLocation, 1, false, worldCamera.GetViewMatrix().get() ); //TODO: multiply by model matrix if needed
    glUniformMatrix4fv( projectionMatrixLocation, 1, false, worldCamera.GetProjectionMatrix().get() );
    glUniformMatrix4fv( normalMatrixLocation, 1, false, inverseModelViewMatrix.getTranspose() );

    //glBindBuffer( GL_ARRAY_BUFFER, positionVbo );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexBuffer );

    glDrawElements( GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_BYTE, 0 );

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

gs::Globe::Globe()
    :   shader( "test", "data/shaders/test.vert", "data/shaders/test.frag" )
{
    VoronoiGenerator vg;
    vg.generateTessellation( 1000 );

    for (auto& cell : vg.cell_vector)
    {
        cerr << cell->centroid.x << " " << cell->centroid.y << " " << cell->centroid.z << endl;
    }

    /* The four vericies of a tetrahedron */
    const GLfloat tetrahedron[4][3] = {
    {  1.0,  1.0,  1.0 },   /* index 0 */
    { -1.0, -1.0,  1.0 },   /* index 1 */
    { -1.0,  1.0, -1.0 },   /* index 2 */
    {  1.0, -1.0, -1.0 } }; /* index 3 */

    const GLfloat colors[4][3] = {
    {  1.0,  0.0,  0.0  }, /* Red */
    {  0.0,  1.0,  0.0  }, /* Green */
    {  0.0,  0.0,  1.0  }, /* Blue */
    {  1.0,  1.0,  1.0  } }; /* White */

    const GLfloat texCoords[4][2] = {
    { 0.0, 0.0 },
    { 0.0, 0.0 },
    { 0.0, 0.0 },
    { 0.0, 0.0 } };

    const GLfloat fog[4] = { 1.0, 1.0, 0.0, 0.0 };

    const GLubyte indices[6] = { 0, 1, 2, 3, 0, 1 };
    //const GLubyte indices[4] = { 0, 1, 2, 3 };

    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    positionVbo = CreateVbo( tetrahedron, 12, 3, "positionVert" );
    colorVbo = CreateVbo( colors, 12, 3, "colorVert" );
    texCoordVbo = CreateVbo( texCoords, 12, 2, "texCoordVert" );
    fogVbo = CreateVbo( fog, 12, 1, "fogVert" );


    //indices
    glGenBuffers( 1, &indexBuffer );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexBuffer );

    glBufferData( GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLubyte), indices, GL_STATIC_DRAW );

    shader.Link();

    modelViewMatrixLocation = shader.GetUniformLocation( "modelViewMatrix" );
    projectionMatrixLocation = shader.GetUniformLocation( "projectionMatrix" );
    normalMatrixLocation = shader.GetUniformLocation( "normalMatrix" );

    //index array for vertices
    //vertex array for normals



}
