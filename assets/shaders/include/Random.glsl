float random(vec2 uv)
{
    return fract(sin(dot(uv, vec2(12.9898, 78.233))) * 43758.5453123);
}

float random(float x)
{
    return random(vec2(x));
}

vec3 randomColor(float x)
{
    return vec3(random(x) * 0.7 + 0.3, random(x + 1.0) * 0.7 + 0.3, random(x + 2.0) * 0.7 + 0.3);
}
