#version 150 core

precision highp float;

smooth  in vec3     normalFrag;
flat    in vec3     colorFrag;
flat    in float    fogFrag;
flat    in vec3     lightVec;

out vec4 colorOut;

void main()
{
    //float atmosphere = (gl_FragCoord.z < 0.095) ? 0 : (gl_FragCoord.z-0.095) / 0.005;
    float atmosphere = max(0, (gl_FragCoord.z-0.095) / 0.005);
    atmosphere = atmosphere * atmosphere;

    float shadow = dot(normalFrag, normalize(lightVec)); //multiply this by some function of zoom
    //shadow = (shadow);

    colorOut = (vec4(colorFrag, 1.0) + vec4(0.4, 0.6, 1.0, 1.0) * atmosphere / 1.0) * shadow ;
}
