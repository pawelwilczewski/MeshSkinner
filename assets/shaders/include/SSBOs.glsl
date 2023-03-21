#include "Bone.glsl"

layout (std430, binding = 1) buffer ss_Transforms { restrict readonly mat4 transforms[]; };
layout (std430, binding = 2) buffer ss_Bones { restrict readonly Bone bones[]; };
layout (std430, binding = 3) buffer ss_FinalPos { restrict writeonly vec4 positions[]; };
