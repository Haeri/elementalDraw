#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec2 uv_varying;
layout(location = 1) in flat int instance_index;

layout(location = 0) out vec4 outColor;

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
layout(set = 0, binding = 1) uniform sampler2D textures[10];


float ellipse_distance(vec2 uv, vec2 center, vec2 dims)
{
    return pow((uv.x-center.x),2)/pow(dims.x,2) + pow((uv.y-center.y),2)/pow(dims.y,2);
}

void main()
{
    //float a = sbo.payload[instance_index].fill_color.a;
    
    vec2 nw = sbo.payload[instance_index].border_radius[0].xy;
    vec2 ne = sbo.payload[instance_index].border_radius[0].zw;
    vec2 se = sbo.payload[instance_index].border_radius[1].xy;
    vec2 sw = sbo.payload[instance_index].border_radius[1].zw;
    
    vec4 fill_color = sbo.payload[instance_index].fill_color.rgba; 
    float line_width = sbo.payload[instance_index].stroke_size_color.x;
    vec3 stroke_color = sbo.payload[instance_index].stroke_size_color.yzw;
    int index = int(sbo.payload[instance_index].sampler_index.x);
    int use_color = int(sbo.payload[instance_index].sampler_index.y);

    vec2 line_width2 = vec2(line_width, line_width);

    nw = clamp(nw, line_width2, vec2(.5));
    ne = clamp(ne, line_width2, vec2(.5));
    se = clamp(se, line_width2, vec2(.5));
    sw = clamp(sw, line_width2, vec2(.5));

    float dist = 0.0;
    float dist2 = 0.0;

    if(uv_varying.x < nw.x && uv_varying.y < nw.y)
    { 
        dist = ellipse_distance(uv_varying, nw, nw);
        dist = step(dist, 1.);
        if(line_width != 0.)
        {            
            dist2 = 1.-ellipse_distance(uv_varying, nw, nw-line_width2);
            dist2 = step(dist2, 0.);
            dist = min(dist, dist2);
        }
    }    
    else if (1.0 -  uv_varying.x < ne.x && uv_varying.y < ne.y)
    {
        dist = ellipse_distance(uv_varying, vec2(1.0-ne.x, ne.y), ne);
        dist = step(dist, 1.);
        if(line_width != 0.)
        { 
            dist2 = 1.-ellipse_distance(uv_varying, vec2(1.0-ne.x, ne.y), ne-line_width2);
            dist2 = step(dist2, 0.);
            dist = min(dist, dist2);
        }
    }
    else if (uv_varying.x < sw.x && 1.0 - uv_varying.y < sw.y)
    {
        dist = ellipse_distance(uv_varying, vec2(sw.x, 1.0-sw.y), sw);
        dist = step(dist, 1.);
        if(line_width != 0.)
        { 
            dist2 = 1.-ellipse_distance(uv_varying, vec2(sw.x, 1.0-sw.y), sw-line_width2);
            dist2 = step(dist2, 0.);
            dist = min(dist, dist2);
        }
    }
    else if (1.0 -  uv_varying.x < se.x && 1.0 - uv_varying.y < se.y)
    {
        dist = ellipse_distance(uv_varying, vec2(1.0-se.x, 1.0-se.y), se);
        dist = step(dist, 1.);
        if(line_width != 0.)
        { 
            dist2 = 1.-ellipse_distance(uv_varying, vec2(1.0-se.x, 1.0-se.y), se-line_width2);
            dist2 = step(dist2, 0.);
            dist = min(dist, dist2);
        }
    }
    else
    {
        if(line_width != 0.)
        {            
            if(uv_varying.x <= line_width2.x || uv_varying.y <= line_width2.y || uv_varying.x >= 1. - line_width2.x || uv_varying.y >= 1. - line_width2.y)
            {
                dist = 1.;
            }
        }
        else
        {
            dist = 1.;
        }
    }

    dist = 1.0 - dist;
    float delta = fwidth(dist);
    float alpha = smoothstep(1+delta, 1, dist); 

    if(line_width != 0){
    	outColor = vec4(stroke_color, alpha);	
    }
    else if (index <= -1)
    {
        outColor = vec4(fill_color.rgb, min(alpha, fill_color.a));
    }
    else
    {
    	vec4 img = texture(textures[index], uv_varying.xy).rgba;
    	if(use_color == 1){
	        outColor = vec4((img.rgb * fill_color.rgb), min(alpha, img.a));
    	}else{
    		outColor = vec4((img.rgb), min(alpha, img.a));    		
    	}
    }
}