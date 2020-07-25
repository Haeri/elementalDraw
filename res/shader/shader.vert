#version 450
#extension GL_ARB_separate_shader_objects : enable

out gl_PerVertex{
    vec4 gl_Position;
};

layout(location = 0) in vec2 position;
//layout(location = 1) in vec2 uv;
//layout(location = 1) in vec4 colors;
//layout(location = 2) in vec4 border_radius;

layout(location = 0) out vec2 uv_varying;
layout(location = 1) out vec4 color_varying;

struct UniformData
{
  vec2 vertices[4];
  vec2 border_radius[4];
};

layout(set = 0, binding = 0) uniform UBO
{
  UniformData payload[20];
} ubo;

vec4 _colors[4] = vec4[](
    vec4(1, 0, 0, 1),
    vec4(0, 1, 0, 1),
    vec4(0, 0, 1, 1),
    vec4(0, 1, 1, 1)
);

vec2 _positions[4] = vec2[](
    vec2(0, 0),
    vec2(1, 0),
    vec2(0, 1),
    vec2(1, 1)
);

vec2 uvs[4] = vec2[](
    vec2(0.0),
    vec2(1, 0),
    vec2(0, 1),
    vec2(1)
);

void main()
{
    //gl_Position = vec4(position, 0.0, 1.0);
    gl_Position = vec4(ubo.payload[gl_InstanceIndex].vertices[gl_VertexIndex], 0.0, 1.0);
    uv_varying = uvs[gl_VertexIndex];
    //color_varying = colors;
    color_varying = _colors[gl_VertexIndex];
}