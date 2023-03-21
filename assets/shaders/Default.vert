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
#include "include/Random.glsl"

out vec3 io_Color;

void main()
{	
	io_Color = randomColor(float(in_TransformIndex));

	vec4 worldPosition = transforms[in_TransformIndex] * vec4(in_Position, 1.0);
	positions[gl_VertexID] = worldPosition;
	gl_Position = u_ViewProjection * worldPosition;
}
