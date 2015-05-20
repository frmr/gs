#include "gsGlobe.h"

void gs::Globe::Draw() const
{
    shader.Use();
    glEnableVertexAttribArray( 0 );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );

    glDrawArrays(GL_LINE_LOOP, 0, 4);

}

gs::Globe::Globe()
    :   shader( "test", "data/shaders/test.vert", "data/shaders/test.frag" )
{
    /* We're going to create a simple diamond made from lines */
    const GLfloat diamond[4][2] = {
    {  0.0,  1.0  }, /* Top point */
    {  1.0,  0.0  }, /* Right point */
    {  0.0, -1.0  }, /* Bottom point */
    { -1.0,  0.0  } }; /* Left point */

    const GLfloat colors[4][3] = {
    {  1.0,  0.0,  0.0  }, /* Red */
    {  0.0,  1.0,  0.0  }, /* Green */
    {  0.0,  0.0,  1.0  }, /* Blue */
    {  1.0,  1.0,  1.0  } }; /* White */

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);


    GLuint loc = shader.GetAttribLocation("in_Position");

    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), diamond, GL_STATIC_DRAW);
    glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    shader.Link();





    //index array for vertices
    //vertex array for normals
}
