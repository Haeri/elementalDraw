#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec2 uv_varying;
layout(location = 1) in vec4 color_varying;

layout(location = 0) out vec4 outColor;

struct UniformData
{
  vec2 vertices[4];
  vec2 border_radius[4];
};

layout(set = 0, binding = 0) uniform UBO
{
  UniformData payload[20];
} ubo;

bool ellipse_check(vec2 uv, vec2 center, vec2 dims)
{
    return pow((uv.x-center.x),2)/pow(dims.x,2) + pow((uv.y-center.y),2)/pow(dims.y,2) <= 0;
}

void main()
{
    float a = 1;
    //float r = 0.1;
    /*
    vec2 nw = ubo.payload[gl_InstanceIndex].border_radius[0];
    vec2 ne = ubo.payload[gl_InstanceIndex].border_radius[1];
    vec2 se = ubo.payload[gl_InstanceIndex].border_radius[2];
    vec2 sw = ubo.payload[gl_InstanceIndex].border_radius[3];

    if(
    (ellipse_check(uv_varying, nw, nw*2) &&      uv_varying.x < nw.x &&     uv_varying.y < nw.y) ||
    (ellipse_check(uv_varying, vec2(1-r, r)) && 1 -  uv_varying.x < ne.x &&     uv_varying.y < ne.y) ||
    (ellipse_check(uv_varying, vec2(r,   1-r)) &&      uv_varying.x < se.x && 1 - uv_varying.y < se.y) ||
    (ellipse_check(uv_varying, vec2(1-r, 1-r)) && 1 -  uv_varying.x < sw.x && 1 - uv_varying.y < sw.y)
    )
    {
        a = 0;
    }
    */
    outColor = vec4(color_varying.rgb, a);
}