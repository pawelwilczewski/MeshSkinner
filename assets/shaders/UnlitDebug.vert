#version 450 core

//#include "include/Material.glsl"
#include "include/VertexInfo.glsl"

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec2 in_TexCoord;
layout(location = 2) in vec3 in_Normal;
layout(location = 3) in vec3 in_Color;
layout(location = 4) in uvec4 in_Bones;
layout(location = 5) in vec4 in_Weights;

layout (std430, binding = 0) buffer ss_VertexInfo { VertexInfo vertexInfo[]; };
layout (std430, binding = 1) buffer ss_Transforms { mat4 transforms[]; };
//layout (std430, binding = 2) buffer ss_Materials { restrict readonly Material Materials[]; };

uniform mat4 u_ViewProjection;

out vec3 io_Color;

void main()
{
	io_Color = in_Color;
	vec4 worldPosition = transforms[vertexInfo[gl_VertexID].transformID] * vec4(in_Position, 1.0);
	gl_Position = u_ViewProjection * worldPosition;
}
