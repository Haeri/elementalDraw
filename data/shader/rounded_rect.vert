#version 450
#extension GL_ARB_separate_shader_objects : enable

out gl_PerVertex{
    vec4 gl_Position;
};

layout(location = 0) out vec2 uv_varying;
layout(location = 1) out vec2 uv_tex_varying;
layout(location = 2) out flat int instance_index;

struct StorageData
{
    vec4 fill_color;
    vec4 vertices[2];
    vec4 border_radius[2];
    vec4 sampler_index; 
    vec4 stroke_size_color;
    vec4 uvs;
};

layout(set = 0, binding = 0, std140) readonly buffer SBO
{
    StorageData payload[];
} sbo;

vec2 _positions[4] = vec2[](
    vec2(0, 0),
    vec2(1, 0),
    vec2(0, 1),
    vec2(1, 1)
);

void main()
{
    vec2 verts[4] = {
        sbo.payload[gl_InstanceIndex].vertices[0].xy,
        sbo.payload[gl_InstanceIndex].vertices[0].zw,
        sbo.payload[gl_InstanceIndex].vertices[1].xy,
        sbo.payload[gl_InstanceIndex].vertices[1].zw
    };

    vec2 _uvs[4] = vec2[](
        vec2(sbo.payload[gl_InstanceIndex].uvs.x, sbo.payload[gl_InstanceIndex].uvs.y),
        vec2(sbo.payload[gl_InstanceIndex].uvs.z, sbo.payload[gl_InstanceIndex].uvs.y),
        vec2(sbo.payload[gl_InstanceIndex].uvs.x, sbo.payload[gl_InstanceIndex].uvs.w),
        vec2(sbo.payload[gl_InstanceIndex].uvs.z, sbo.payload[gl_InstanceIndex].uvs.w)
    );

    instance_index = gl_InstanceIndex;
    gl_Position = vec4(verts[gl_VertexIndex].xy, 0.0, 1.0);
    uv_varying = _positions[gl_VertexIndex];
    uv_tex_varying = _uvs[gl_VertexIndex];
}