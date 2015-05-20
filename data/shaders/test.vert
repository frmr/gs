#version 150
// in_Position was bound to attribute index 0 and in_Color was bound to attribute index 1
attribute vec2 in_Position;
 
void main(void)
{
    // Since we are using flat lines, our input only had two points: x and y.
    // Set the Z coordinate to 0 and W coordinate to 1
 
    gl_Position = vec3(in_Position, 0.0);
}