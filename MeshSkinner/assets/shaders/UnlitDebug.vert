#version 450 core

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec2 in_TexCoord;
layout(location = 2) in vec3 in_Normal;
layout(location = 3) in vec3 in_Color;

uniform mat4 u_ViewProjection;

out vec3 io_Color;

void main()
{
	io_Color = in_Color;
	gl_Position = u_ViewProjection * vec4(in_Position, 1.0);
}
