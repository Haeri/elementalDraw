#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec2 uv_varying;
layout(location = 1) in flat int instance_index;

layout(location = 0) out vec4 outColor;

struct UniformData
{
    vec4 fill_color;
    vec4 vertices[2];
    vec4 border_radius[2];
    vec4 sampler_index;
    vec4 stroke_size_color;	
};

layout(set = 0, binding = 0, std140) uniform UBO
{
    UniformData payload[65536];
} ubo;
layout(set = 0, binding = 1) uniform sampler2D textures[32];


float ellipse_distance(vec2 uv, vec2 center, vec2 dims)
{
    return pow((uv.x-center.x),2)/pow(dims.x,2) + pow((uv.y-center.y),2)/pow(dims.y,2);
}

void main()
{
    float a = ubo.payload[instance_index].fill_color.a;
    
    vec2 nw = ubo.payload[instance_index].border_radius[0].xy;
    vec2 ne = ubo.payload[instance_index].border_radius[0].zw;
    vec2 se = ubo.payload[instance_index].border_radius[1].xy;
    vec2 sw = ubo.payload[instance_index].border_radius[1].zw;
    
    float line_width = ubo.payload[instance_index].stroke_size_color.x;
    vec3 stroke_color = ubo.payload[instance_index].stroke_size_color.yzw;

    float dist = 0.0;

    if(uv_varying.x < nw.x && uv_varying.y < nw.y)
    { 
        dist = ellipse_distance(uv_varying, nw, nw);
    }
    else if (1 -  uv_varying.x < ne.x &&     uv_varying.y < ne.y){
        dist = ellipse_distance(uv_varying, vec2(1-ne.x, ne.y), ne);
    }
    else if (uv_varying.x < sw.x && 1 - uv_varying.y < sw.y){
        dist = ellipse_distance(uv_varying, vec2(sw.x, 1-sw.y), sw);
    }
    else if (1 -  uv_varying.x < se.x && 1 - uv_varying.y < se.y){
        dist = ellipse_distance(uv_varying, vec2(1-se.x, 1-se.y), se);
    }


    // Stroke Rect
    if(line_width != 0){
    	if(uv_varying.x > line_width && uv_varying.y > line_width && uv_varying.x < 1 - line_width && uv_varying.y < 1 - line_width){
			dist = 1;
    	}
    }


    /* Border Rendering
    vec2 border_width = vec2(0.01);

    if( !(uv_varying.x - border_width.x < 0)
        && !(uv_varying.x + border_width.x > 1)
        && !(uv_varying.y - border_width.y < 0 )
        && !(uv_varying.y + border_width.y > 1))
    {
        discard;
    }
    */

    
    float delta = fwidth(dist);
    a = smoothstep(1+delta, 1, dist); 
    int index = int(ubo.payload[instance_index].sampler_index.r);
    if(line_width != 0){
    	outColor = vec4(stroke_color, a);	
    }
    else if (index <= -1)
    {
        outColor = vec4(ubo.payload[instance_index].fill_color.rgb, a);
    }
    else
    {
        outColor = vec4(texture(textures[index], uv_varying.xy).rgb, a);
    }
    //outColor = vec4(vec3(a), 1);
}