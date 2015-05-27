#version 150

precision highp float;

in vec4 position;
in vec3 colorVert;
out vec3 colorFrag;

uniform mat4 projection;
uniform mat4 modelView;

void main(void)
{
    gl_Position = projection * modelView * position;
    colorFrag = colorVert;
}