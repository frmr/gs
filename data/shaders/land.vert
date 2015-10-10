#version 150 core

precision highp float;

in vec3     positionVert;
in vec3     colorVert;
in vec2     texCoordVert;
in float    fogVert;

smooth  out vec3    normalFrag;
flat    out vec3    colorFrag;
smooth  out vec2    texCoordFrag;
flat    out float   fogFrag;
flat    out vec3    lightVec;

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
    lightVec = (vec4(1.0, 1.0, 3.0, 1.0) * modelViewMatrix).xyz;
}
