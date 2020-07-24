#version 450
#extension GL_ARB_separate_shader_objects : enable

out gl_PerVertex{
    vec4 gl_Position;
};

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec4 colors;
layout(location = 3) in vec4 border_radius;

layout(location = 0) out vec2 uv_varying;
layout(location = 1) out vec4 color_varying;

/*
layout(binding = 0) uniform UBO
{
    vec2 context_dimensions;
} ubo;
*/

void main()
{
    gl_Position = vec4(position, 0.0, 1.0);
    uv_varying = uv;
    color_varying = colors;
}