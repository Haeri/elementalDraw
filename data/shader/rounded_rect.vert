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
    vec4 color;										// 32   4
    vec4 vertices[2];								// 64   4, 4
    vec4 border_radius;								// 32   4
    vec4 sampler_index1__use_tint1__resolution2; 	// 32   1:1:2
    vec4 uvs;										// 32   4
    vec4 line_width;    							// 32   4
	vec4 shadow_size;      							// 32   1:0:0:0
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