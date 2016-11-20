#version 150 core

precision highp float;

smooth  in vec3     normalFrag;
flat    in vec3     colorFrag;
smooth  in vec2     texCoordFrag;
flat    in float    fogFrag;
flat    in vec3     lightVec;

uniform sampler2D   tex;

out vec4 colorOut;


void main()
{
	float atmosphere = max(0, (gl_FragCoord.z-0.095) / 0.005);
	atmosphere = atmosphere * atmosphere;
	
	float shadow = dot(normalFrag, normalize(lightVec));
	
	vec4 landColor = texture(tex, texCoordFrag);
	
    //political map mode
    landColor = vec4((landColor.rgb + colorFrag) / 2.0, 1.0);

    //terrain map mode
    colorOut = (landColor + vec4(0.4, 0.6, 1.0, 1.0) * atmosphere / 1.0) * shadow;
}
