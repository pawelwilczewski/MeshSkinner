#version 450 core

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Normal;
layout(location = 2) in vec4 in_Tangent;
layout(location = 3) in vec2 in_TexCoord;
layout(location = 4) in vec3 in_Color;
layout(location = 5) in vec4 in_Bones;
layout(location = 6) in vec4 in_Weights;
layout(location = 7) in uint in_TransformIndex;
layout(location = 8) in uint in_SkeletonBonesIndex;
layout(location = 9) in uint in_SkeletonTransformsIndex;

#include "include/SSBOs.glsl"
#include "include/Uniforms.glsl"

out vec3 io_Color;

void main()
{
	if (u_SelectedEntity != in_TransformIndex)
		io_Color = vec3(0.7);
	else
	{
		float weight = 0.0;
		if (in_Bones[0] == u_SelectedBone)
			weight += in_Weights[0];
		else if (in_Bones[1] == u_SelectedBone)
			weight += in_Weights[1];
		else if (in_Bones[2] == u_SelectedBone)
			weight += in_Weights[2];
		else if (in_Bones[3] == u_SelectedBone)
			weight += in_Weights[3];

		vec3 blue = vec3(0.0, 0.0, 1.0);
		vec3 cyan = vec3(0.0, 1.0, 1.0);
		vec3 green = vec3(0.0, 1.0, 0.0);
		vec3 yellow = vec3(1.0, 1.0, 0.0);
		vec3 red = vec3(1.0, 0.0, 0.0);

		float transitionSize = 1.0 / 4;

		if	(weight < 1.0 * transitionSize)		io_Color = mix(blue,	cyan,	(weight - 0.0 * transitionSize) / transitionSize);
		else if (weight < 2.0 * transitionSize)	io_Color = mix(cyan,	green,	(weight - 1.0 * transitionSize) / transitionSize);
		else if (weight < 3.0 * transitionSize)	io_Color = mix(green,	yellow,	(weight - 2.0 * transitionSize) / transitionSize);
		else									io_Color = mix(yellow,	red,	(weight - 3.0 * transitionSize) / transitionSize);
	}

	mat4 inverseRootTransform = inverse(transforms[in_SkeletonTransformsIndex]);
	mat4 poseMatrix = mat4(0.0);
	for (int i = 0; i < 4; i++)
	{
		if (in_Weights[i] < 0.000001)
			continue;

		Bone bone = bones[in_SkeletonBonesIndex + int(in_Bones[i])];
		mat4 boneModelTransform = inverseRootTransform * transforms[in_SkeletonTransformsIndex + int(in_Bones[i])];
		poseMatrix += boneModelTransform * bone.inverseBindMatrix * in_Weights[i];
	}

	vec4 worldPosition = transforms[in_TransformIndex] * poseMatrix * vec4(in_Position, 1.0);
	positions[gl_VertexID] = worldPosition;
	gl_Position = u_ViewProjection * worldPosition;
}
