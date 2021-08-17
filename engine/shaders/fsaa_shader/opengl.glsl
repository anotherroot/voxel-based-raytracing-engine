#type vertex
#version 420



layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_tex_coords;




out vec2 v_tex_coords;



void main()
{
    v_tex_coords = a_tex_coords;
    gl_Position =  vec4(a_position,1);
}
#type fragment
#version 420

layout(location = 0) out vec4 out_color;


in vec2 v_tex_coords;

uniform sampler2D u_input;
uniform vec2 u_resolution;

const float FXAA_SPAN_MAX = 8.0;
const float FXAA_REDUCE_MUL = 1.0 / 8.0;
const float FXAA_REDUCE_MIN = 1.0 / 128.0;

void main () {
    vec2 delta = 1.0 / u_resolution;

    vec3 rgbNW = texture(u_input, v_tex_coords + vec2(-1.0, -1.0) * delta).rgb;
    vec3 rgbNE = texture(u_input, v_tex_coords + vec2(1.0, -1.0) * delta).rgb;
    vec3 rgbSW = texture(u_input, v_tex_coords + vec2(-1.0, 1.0) * delta).rgb;
    vec3 rgbSE = texture(u_input, v_tex_coords + vec2(1.0, 1.0) * delta).rgb;
    vec3 rgbM = texture(u_input, v_tex_coords).rgb;

    vec3 luma = vec3(0.299, 0.587, 0.114);
    float lumaNW = dot(rgbNW, luma);
    float lumaNE = dot(rgbNE, luma);
    float lumaSW = dot(rgbSW, luma);
    float lumaSE = dot(rgbSE, luma);
    float lumaM  = dot(rgbM,  luma);

    float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));
    float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE)));

    vec2 dir = vec2(
    -((lumaNW + lumaNE) - (lumaSW + lumaSE)),
    ((lumaNW + lumaSW) - (lumaNE + lumaSE)));

    float dirReduce = max((lumaNW + lumaNE + lumaSW + lumaSE) * (0.25 * FXAA_REDUCE_MUL), FXAA_REDUCE_MIN);
    float rcpDirMin = 1.0 / (min(abs(dir.x), abs(dir.y)) + dirReduce);
    dir = min(vec2(FXAA_SPAN_MAX), max(vec2(-FXAA_SPAN_MAX), dir * rcpDirMin)) * delta.xy;

    vec3 rgbA = 0.5 * (texture(u_input, v_tex_coords.xy + dir * (1.0 / 3.0 - 0.5)).xyz + texture(u_input, v_tex_coords.xy +
    dir * (2.0 / 3.0 - 0.5)).xyz);
    vec3 rgbB = rgbA * 0.5 + 0.25 * (texture(u_input, v_tex_coords.xy + dir * -0.5).xyz + texture(u_input, v_tex_coords.xy
    + dir * 0.5).xyz);
    float lumaB = dot(rgbB, luma);
    if (lumaB < lumaMin || lumaB > lumaMax) {
        out_color = vec4(rgbA, 1.0);
    } else {
        out_color = vec4(rgbB, 1.0);
    }
}
