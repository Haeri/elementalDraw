// https://thebookofshaders.com/edit.php

#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform float u_time;

float ellipse_distance(vec2 uv, vec2 center, vec2 dims)
{
    return pow((uv.x-center.x),2.0)/pow(dims.x,2.0) + pow((uv.y-center.y),2.0)/pow(dims.y,2.0);
}

void main() {
    vec2 res = u_resolution;
    vec2 uv_varying = gl_FragCoord.xy/res.xy;
    
    float line_width = 80.992;
    vec2 line_width2 = line_width / res;
    
    vec2 nw = clamp(vec2(0.520,0.550), line_width2, vec2(.5));
    vec2 ne = clamp(vec2(0.3), line_width2, vec2(.5));
    vec2 se = clamp(vec2(0.110,0.050), line_width2, vec2(.5));
    vec2 sw = clamp(vec2(0.710,0.380), line_width2, vec2(.5));
    
    vec3 stroke_color = vec3(0.000,0.260,0.895);
    
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
                stroke_color = vec3(1.000,0.686,0.259);
            }
        }
        else
        {
            dist = 1.;
        }
    }
    
    
    //float delta = dist;
    //float alpha = smoothstep(-0.040+delta, 1.0, dist); 
    //dist = step(dist, 1.);
    

    gl_FragColor = vec4(stroke_color.rgb, dist);
}