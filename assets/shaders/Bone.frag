#version 450 core

layout(location = 0) out vec4 out_Color;

in vec3 io_Color;

void main()
{
	out_Color = vec4(io_Color, 0.5);
}
