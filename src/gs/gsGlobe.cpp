#include "gsGlobe.h"

#include <iostream>

using std::cerr;
using std::endl;

void gs::Globe::Draw( const gs::Camera& worldCamera ) const
{
    shader.Use();
    glEnableVertexAttribArray( 0 );

    glUniformMatrix4fv( modelViewLocation, 1, false, worldCamera.GetViewMatrix().get() ); //TODO: multiply by model matrix if needed
    glUniformMatrix4fv( projectionLocation, 1, false, worldCamera.GetProjectionMatrix().get() );

    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexBuffer );

    glDrawElements( GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_BYTE, 0 );

    //glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    //glDrawArrays(GL_LINE_LOOP, 0, 4);
}

void gs::Globe::Update()
{

}

gs::Globe::Globe()
    :   shader( "test", "data/shaders/test.vert", "data/shaders/test.frag" )
{
//    /* We're going to create a simple diamond made from lines */
//    const GLfloat diamond[4][2] = {
//    {  0.0,  1.0  }, /* Top point */
//    {  1.0,  0.0  }, /* Right point */
//    {  0.0, -1.0  }, /* Bottom point */
//    { -1.0,  0.0  } }; /* Left point */

    const GLfloat diamond[4][4] = {
    {  0.0,  1.0, 0.0, 1.0  }, /* Top point */
    {  1.0,  0.0, 0.0, 1.0  }, /* Right point */
    {  0.0, -1.0, 0.0, 1.0  }, /* Bottom point */
    { -1.0,  0.0, 0.0, 1.0  } }; /* Left point */

    /* The four vericies of a tetrahedron */
    const GLfloat tetrahedron[4][4] = {
    {  1.0,  1.0,  1.0, 1.0  },   /* index 0 */
    { -1.0, -1.0,  1.0, 1.0  },   /* index 1 */
    { -1.0,  1.0, -1.0, 1.0  },   /* index 2 */
    {  1.0, -1.0, -1.0, 1.0  } }; /* index 3 */

    const GLfloat colors[4][3] = {
    {  1.0,  0.0,  0.0  }, /* Red */
    {  0.0,  1.0,  0.0  }, /* Green */
    {  0.0,  0.0,  1.0  }, /* Blue */
    {  1.0,  1.0,  1.0  } }; /* White */

    const GLubyte indices[6] = { 0, 1, 2, 3, 0, 1 };
    //const GLubyte indices[4] = { 0, 1, 2, 3 };

    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    //positions
    glGenBuffers( 1, &vbo );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );

    GLuint positionLocation = shader.GetAttribLocation( "position" );

    glBufferData( GL_ARRAY_BUFFER, 16 * sizeof(GLfloat), tetrahedron, GL_STATIC_DRAW );
    glVertexAttribPointer( positionLocation, 4, GL_FLOAT, GL_FALSE, 0, 0 );
    glEnableVertexAttribArray( positionLocation );

    //colors
    glGenBuffers( 1, &colorBuffer );
    glBindBuffer( GL_ARRAY_BUFFER, colorBuffer );

    GLuint colorLocation = shader.GetAttribLocation( "colorVert" );

    glBufferData( GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), colors, GL_STATIC_DRAW );
    glVertexAttribPointer( colorLocation, 3, GL_FLOAT, GL_FALSE, 0, 0 );
    glEnableVertexAttribArray( colorLocation );

    //indices
    glGenBuffers( 1, &indexBuffer );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexBuffer );

    glBufferData( GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLubyte), indices, GL_STATIC_DRAW );

    shader.Link();

    modelViewLocation = shader.GetUniformLocation( "modelView" );
    projectionLocation = shader.GetUniformLocation( "projection" );

    //index array for vertices
    //vertex array for normals
}
