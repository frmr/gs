#version 150

precision highp float;

smooth in vec3  normalFrag;
flat in vec3    colorFrag;
smooth in vec2  texCoordFrag;
flat in float   fogFrag;

void main(void)
{
    gl_FragColor = vec4( colorFrag, 1.0 ) * dot(normalFrag, vec3(1.0,0.0,0.0));
}