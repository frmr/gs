#include "gsGlobe.h"

#include <iostream>

using std::cerr;
using std::endl;

void gs::Globe::Draw( const gs::MatrixStack<Matrix4>& modelView, const gs::ProjectionMatrix& projection ) const
{
    shader.Use();
    glEnableVertexAttribArray( 0 );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );

    glUniformMatrix4fv( modelViewLocation, 1, false, modelView.top.get() );
    glUniformMatrix4fv( projectionLocation, 1, false, projection.get() );

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
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

    const GLfloat colors[4][3] = {
    {  1.0,  0.0,  0.0  }, /* Red */
    {  0.0,  1.0,  0.0  }, /* Green */
    {  0.0,  0.0,  1.0  }, /* Blue */
    {  1.0,  1.0,  1.0  } }; /* White */

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);


    GLuint loc = shader.GetAttribLocation("position");

    glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(GLfloat), diamond, GL_STATIC_DRAW);
    glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    //glBindFragDataLocation( shader.program, 0, "outputF" );

    shader.Link();

    modelViewLocation = shader.GetUniformLocation( "modelView" );
    projectionLocation = shader.GetUniformLocation( "projection" );



    //index array for vertices
    //vertex array for normals
}
