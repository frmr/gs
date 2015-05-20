#version 150
// It was expressed that some drivers required this next line to function properly
//precision highp float;

//out vec4 gl_FragColor;
 
void main(void)
{
    // Pass through our original color with full opacity.
    gl_FragColor = vec3(0.0, 1.0, 0.0);
}