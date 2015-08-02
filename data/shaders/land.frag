#version 150 core

precision highp float;

smooth  in vec3     normalFrag;
flat    in vec3     colorFrag;
smooth  in vec2     texCoordFrag;
flat    in float    fogFrag;

uniform sampler2D   tex;

out vec4 colorOut;

void main()
{
    //colorOut = vec4( colorFrag, 1.0 );
    //colorOut = texture( tex, texCoordFrag );
    colorOut = vec4( texCoordFrag, 0, 0 );
}