#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec2 uv_varying;
layout(location = 1) in vec4 color_varying;

layout(location = 0) out vec4 outColor;

void main()
{
    float a = 1;
    float r = 0.25;
    if(
    (distance(uv_varying, vec2(r))          > r && uv_varying.x     < r && uv_varying.y        < r) ||
    (distance(uv_varying, vec2(1-r, r))     > r && uv_varying.x - 1 < r && uv_varying.y        < r) ||
    (distance(uv_varying, vec2(r,   1-r))   > r && uv_varying.x     < r && uv_varying.y - 1    < r) ||
    (distance(uv_varying, vec2(1-r, 1-r))   > r && uv_varying.x - 1 < r && uv_varying.y - 1    < r)
    )
    {
        a = 0;
    }

    outColor = vec4(vec3(a), 1);
}