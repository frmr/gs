#version 150 core

precision highp float;

smooth  in vec3     normalFrag;
flat    in vec3     colorFrag;
smooth  in vec2     texCoordFrag;
flat    in float    fogFrag;

out vec4 colorOut;

void main()
{
    //gl_FragColor = vec4( colorFrag, 1.0 ) * dot(normalFrag, vec3(1.0,0.0,0.0));
    colorOut = vec4( colorFrag, 1.0 );
    //gl_FragColor = vec4( colorFrag, 1.0 );
    
    //gl_FragColor = vec4(1.0,0.0,0.0,1.0);
}