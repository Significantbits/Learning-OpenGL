#version 330 core
layout (location = 0) in vec3 aPos;
uniform float x_offset;
uniform float y_offset;
out vec4 vColor;
void main()
{
	gl_Position = vec4(aPos.x+x_offset,aPos.y+y_offset,aPos.z,1.0f);
	vColor = vec4(gl_Position.x,gl_Position.y,(gl_Position.x+gl_Position.y)/2,1.0f);
}
