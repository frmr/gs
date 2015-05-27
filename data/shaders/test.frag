#version 150

precision highp float;

in vec3 colorFrag;

void main(void)
{
    //gl_FragColor = vec4(0.0, 0.0, 1.0, 1.0);
    gl_FragColor = vec4( colorFrag, 1.0 );
}