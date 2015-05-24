#version 150

precision highp float;

in vec4 position;

void main(void)
{
    gl_Position = position;// + vec4(10,10,0,1);
}