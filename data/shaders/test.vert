#version 150

precision highp float;

in vec4 position;
in vec3 colorVert;

uniform mat4 projection;
uniform mat4 modelView;

out vec3 colorFrag;

void main(void)
{
    gl_Position = projection * modelView * position;
    //gl_Position = position;
    colorFrag = colorVert;
}