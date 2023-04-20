#version 450 core

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Normal;
layout(location = 2) in vec4 in_Tangent;
layout(location = 3) in vec2 in_TexCoord;
layout(location = 4) in vec3 in_Color;
layout(location = 5) in uint in_TransformIndex;
layout(location = 6) in uint in_SkeletonBonesIndex;
layout(location = 7) in uint in_SkeletonTransformsIndex;

#include "include/SSBOs.glsl"
#include "include/Uniforms.glsl"

out vec3 io_Color;

void main()
{
	if (u_SelectedEntity == in_SkeletonTransformsIndex - 1 && u_SelectedBone == in_TransformIndex - in_SkeletonTransformsIndex)
		io_Color = vec3(0.7, 0.1, 0.8);
	else
		io_Color = vec3(1.0);

	vec4 worldPosition = transforms[in_TransformIndex] * vec4(in_Position, 1.0);
	positions[gl_VertexID] = worldPosition;
	gl_Position = u_ViewProjection * worldPosition;
}
