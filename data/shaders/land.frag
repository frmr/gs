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
    //political map mode
    //colorOut = vec4( ( texture( tex, texCoordFrag ).rgb + colorFrag ) / 2.0, 1.0 );
    
    //terrain map mode
    colorOut = texture( tex, texCoordFrag );
}