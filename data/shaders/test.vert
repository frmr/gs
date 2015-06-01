#version 150

precision highp float;

in vec3     positionVert;
in vec3     colorVert;
in vec2     texCoordVert;
in float    fogVert;

out vec3    normalFrag;
out vec3    colorFrag;
out vec2    texCoordFrag;
out float   fogFrag;

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 normalMatrix;

void main()
{
    gl_Position = projectionMatrix * modelViewMatrix * vec4(positionVert, 1.0);
    //normalFrag = normalize((normalMatrix * vec4(positionVert, 1.0)).xyz);
    normalFrag = normalize(positionVert);
    colorFrag = colorVert;
    texCoordFrag = texCoordVert;
    fogFrag = fogVert;
}