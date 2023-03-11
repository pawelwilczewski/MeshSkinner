#version 450 core

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Normal;
layout(location = 2) in vec4 in_Tangent;
layout(location = 3) in vec2 in_TexCoord;
layout(location = 4) in vec3 in_Color;
layout(location = 5) in uvec4 in_Bones;
layout(location = 6) in vec4 in_Weights;

#include "include/SSBOs.glsl"

uniform mat4 u_ViewProjection;

out vec3 io_Color;

void main()
{
	uint transformID = vertexInfo[gl_VertexID].transformID;
	
	io_Color = in_Color;

	vec4 worldPosition = transforms[transformID] * vec4(in_Position, 1.0);
	gl_Position = u_ViewProjection * worldPosition;
}
