#include "VertexInfo.glsl"
#include "Bone.glsl"

layout (std430, binding = 0) buffer ss_VertexInfo { restrict readonly VertexInfo vertexInfo[]; };
layout (std430, binding = 1) buffer ss_Transforms { restrict readonly mat4 transforms[]; };
layout (std430, binding = 2) buffer ss_Bones { restrict readonly Bone bones[]; };
layout (std430, binding = 3) buffer ss_FinalVertexPositions { vec3 positions[]; };
