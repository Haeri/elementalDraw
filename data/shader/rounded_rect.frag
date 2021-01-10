#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec2 uv_varying;
layout(location = 1) in vec2 uv_tex_varying;
layout(location = 2) in flat int instance_index;

layout(location = 0) out vec4 outColor;

struct RoundedRectData
{
    vec4 color;										// 32   4
    vec4 vertices[2];								// 64   4, 4
    vec4 border_radius;								// 32   4
    vec4 sampler_index1_use_tint1_resolution2; 		// 32   1:1:2
    vec4 uvs;										// 32   4
    vec4 line_width;    							// 32   4
	vec4 shadow_size1_is_msdf1;						// 32   1:0:0:0
};

layout(set = 0, binding = 0, std140) readonly buffer SBO
{
    RoundedRectData payload[];
} sbo;
layout(set = 0, binding = 1) uniform sampler2D textures[10];

float median(float r, float g, float b)
{
    return max(min(r, g), min(max(r, g), b));
}

float sdRoundedBox(vec2 p, vec2 b, vec4 r )
{
    r.xy = (p.x>0.0)?r.xy : r.zw;
    r.x  = (p.y>0.0)?r.x  : r.y;
    vec2 q = abs(p)-b+r.x;
    return min(max(q.x,q.y),0.0) + length(max(q,0.0)) - r.x;
}

void main()
{   
	vec4 border_radius = sbo.payload[instance_index].border_radius * 2.;     
    vec4 color = sbo.payload[instance_index].color; 
    float line_width = sbo.payload[instance_index].line_width.x * 2.;
    int index = int(sbo.payload[instance_index].sampler_index1_use_tint1_resolution2.x);
    int use_tint = int(sbo.payload[instance_index].sampler_index1_use_tint1_resolution2.y);
    vec2 resolution = sbo.payload[instance_index].sampler_index1_use_tint1_resolution2.zw;
    float shadow_size = pow(sbo.payload[instance_index].shadow_size1_is_msdf1.x, 2.);
    int is_msdf = int(sbo.payload[instance_index].shadow_size1_is_msdf1.y);

	float dominant_axis = (resolution.x/resolution.y > 0.) ? resolution.x : resolution.y;
	vec4 border_radius_norm = border_radius/dominant_axis; 
	vec2 resolution_norm = resolution / dominant_axis;
	float line_width_norm = line_width / dominant_axis;
	float shadow_size_norm = shadow_size / dominant_axis;

 	float dist = 1.-(sdRoundedBox((uv_varying-0.5)*2.*(resolution_norm+shadow_size_norm), resolution_norm, border_radius_norm)-shadow_size_norm);
    float bias = min(fwidth(dist), line_width);
    float shape = smoothstep(1., 1.+bias, dist);

	if(shadow_size > 0.)
    {
        shape = smoothstep(1., 1. + shadow_size_norm, dist);
    }
    else if(line_width > 0.)
    {
        float inner = 1.-smoothstep(1.+line_width_norm, 1.+line_width_norm+bias, dist);
        shape = min(inner, shape);
    }

	float alpha = shape;


    if(line_width > 0.)
    {
    	outColor = vec4(color.rgb, alpha);	
    }
    else if (index <= -1)
    {
        outColor = vec4(color.rgb, min(alpha, color.a));
    }
    else
    {
    	vec4 img = texture(textures[index], uv_tex_varying.xy).rgba;

    	if(is_msdf > 0)
		{
		 	vec2 msdfUnit = 4./vec2(textureSize(textures[index], 0));
		    vec3 sampl = texture(textures[index], uv_tex_varying.xy).rgb;
		    float sigDist = median(sampl.r, sampl.g, sampl.b) - 0.5;
		    sigDist *= dot(msdfUnit, 0.5/fwidth(uv_tex_varying.xy));
		    float opacity = clamp(sigDist + 0.5, 0.0, 1.0);
		    outColor = vec4(color.rgb, min(opacity, img.a));
		}
		else
		{
	    	if(use_tint == 1){
		        outColor = vec4((img.rgb * color.rgb), min(alpha, img.a));
	    	}else{
	    		outColor = vec4((img.rgb), min(alpha, img.a));
	    	}
	    }
	}
}