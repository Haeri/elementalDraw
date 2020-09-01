#version 450
#extension GL_ARB_separate_shader_objects : enable

out gl_PerVertex{
    vec4 gl_Position;
};

//layout(location = 0) in vec2 position;

layout(location = 0) out vec2 uv_varying;
layout(location = 1) out flat int instance_index;

struct UniformData
{
    vec4 fill_color;
    vec4 vertices[2];
    vec4 border_radius[2];
};

layout(set = 0, binding = 0, std140) uniform UBO
{
    UniformData payload[1048576];
} ubo;

vec2 _positions[4] = vec2[](
    vec2(0, 0),
    vec2(1, 0),
    vec2(0, 1),
    vec2(1, 1)
);

vec2 _uvs[4] = vec2[](
    vec2(0.0),
    vec2(1, 0),
    vec2(0, 1),
    vec2(1)
);

void main()
{
    vec2 verts[4] = {
        ubo.payload[gl_InstanceIndex].vertices[0].xy,
        ubo.payload[gl_InstanceIndex].vertices[0].zw,
        ubo.payload[gl_InstanceIndex].vertices[1].xy,
        ubo.payload[gl_InstanceIndex].vertices[1].zw
    };

    instance_index = gl_InstanceIndex;
    gl_Position = vec4(verts[gl_VertexIndex].xy, 0.0, 1.0);
    uv_varying = _uvs[gl_VertexIndex];
}