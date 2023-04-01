#version 450 core

layout(location = 0) out vec4 out_Color;

in vec3 io_Color;

uniform float u_FinalAlpha;

void main()
{
	out_Color = vec4(io_Color, u_FinalAlpha);
}
