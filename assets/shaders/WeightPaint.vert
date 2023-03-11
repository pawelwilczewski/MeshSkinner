#version 450 core

//#include "include/Material.glsl"
#include "include/VertexInfo.glsl"
#include "include/Random.glsl"
#include "include/Bone.glsl"

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Normal;
layout(location = 2) in vec4 in_Tangent;
layout(location = 3) in vec2 in_TexCoord;
layout(location = 4) in vec3 in_Color;
layout(location = 5) in vec4 in_Bones;
layout(location = 6) in vec4 in_Weights;

#include "include/SSBOs.glsl"

uniform mat4 u_ViewProjection;
uniform int u_ActiveBone;

out vec3 io_Color;

void main()
{
	float weight = 0.0;
	if (in_Bones[0] == u_ActiveBone)
		weight += in_Weights[0];
	else if (in_Bones[1] == u_ActiveBone)
		weight += in_Weights[1];
	else if (in_Bones[2] == u_ActiveBone)
		weight += in_Weights[2];
	else if (in_Bones[3] == u_ActiveBone)
		weight += in_Weights[3];

	vec3 blue = vec3(0.0, 0.0, 1.0);
	vec3 cyan = vec3(0.0, 1.0, 1.0);
	vec3 green = vec3(0.0, 1.0, 0.0);
	vec3 yellow = vec3(1.0, 1.0, 0.0);
	vec3 red = vec3(1.0, 0.0, 0.0);

	float transitionSize = 1.0 / 4;

	if		(weight < 1.0 * transitionSize)	io_Color = mix(blue,	cyan,	(weight - 0.0 * transitionSize) / transitionSize);
	else if (weight < 2.0 * transitionSize)	io_Color = mix(cyan,	green,	(weight - 1.0 * transitionSize) / transitionSize);
	else if (weight < 3.0 * transitionSize)	io_Color = mix(green,	yellow,	(weight - 2.0 * transitionSize) / transitionSize);
	else									io_Color = mix(yellow,	red,	(weight - 3.0 * transitionSize) / transitionSize);

	mat4 t = transforms[vertexInfo[gl_VertexID].transformID];

	mat4 m = mat4(0.0);
	for (int i = 0; i < 4; i++)
	{
		Bone bone = bones[vertexInfo[gl_VertexID].skeletonID + int(in_Bones[i])];
		m += bone.modelMatrix * bone.inverseBindMatrix * in_Weights[i];
	}

	vec4 worldPosition = t * m * vec4(in_Position, 1.0);
	positions[gl_VertexID] = vec3(worldPosition);

	gl_Position = u_ViewProjection * worldPosition;
}
