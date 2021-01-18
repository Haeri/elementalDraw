#version 410 core

out gl_PerVertex{
	vec4 gl_Position;
};

layout(location = 0) out vec2 uv_varying;
layout(location = 1) out vec2 uv_tex_varying;
layout(location = 2) flat out int instance_index;

struct RoundedRectData
{
	vec4 color;									// 32   4
	vec4 vertices[2];							// 64   4, 4
	vec4 border_radius;							// 32   4
	vec4 sampler_index1_use_tint1_resolution2;	// 32   1:1:2
	vec4 uvs;									// 32   4
	vec4 line_width;							// 32   4
	vec4 shadow_size1_is_msdf1;					// 32   1:0:0:0
};

layout(std140) uniform UBO
{
	RoundedRectData payload[1]; // this is a meme
} ubo;

const vec2 default_uvs[4] = vec2[4](
	vec2(0, 0),
	vec2(1, 0),
	vec2(0, 1),
	vec2(1, 1)
);

void main()
{
	instance_index = gl_InstanceID;

	vec2 verts[4] = vec2[4](
		ubo.payload[instance_index].vertices[0].xy,
		ubo.payload[instance_index].vertices[0].zw,
		ubo.payload[instance_index].vertices[1].xy,
		ubo.payload[instance_index].vertices[1].zw
	);

	vec2 uvs[4] = vec2[4](
		vec2(ubo.payload[instance_index].uvs.x, ubo.payload[instance_index].uvs.y),
		vec2(ubo.payload[instance_index].uvs.z, ubo.payload[instance_index].uvs.y),
		vec2(ubo.payload[instance_index].uvs.x, ubo.payload[instance_index].uvs.w),
		vec2(ubo.payload[instance_index].uvs.z, ubo.payload[instance_index].uvs.w)
	);

	gl_Position = vec4(verts[gl_VertexID].xy*vec2(1.0, -1.0), 0.0, 1.0);
	uv_varying = default_uvs[gl_VertexID];
	uv_tex_varying = uvs[gl_VertexID];
}