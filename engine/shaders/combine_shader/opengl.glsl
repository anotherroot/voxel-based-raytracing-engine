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

uniform vec2 u_window_size;
uniform sampler2D u_albedo;
uniform sampler2D u_diffuse;
uniform sampler2D u_ambient;
uniform sampler2D u_reflection;
uniform sampler2D u_dither;
uniform int u_dithering;

uniform vec3 u_ambient_color;
uniform float u_ambient_start;
uniform float u_ambient_scalar;
uniform float u_diffuse_scalar;


void main()
{
  vec3 albedo = texture(u_albedo, v_tex_coords).xyz;
  vec3 diffuse = texture(u_diffuse, v_tex_coords).xyz;
  vec3 ambient = texture(u_ambient, v_tex_coords).xyz;
  vec4 reflection = texture(u_reflection, v_tex_coords);
  vec3 dither = vec3(texture(u_dither, gl_FragCoord.xy/16.0).r / 32.0 -(1.0/64.0));

  out_color = vec4(albedo*(u_ambient_start*u_ambient_color + u_ambient_scalar*ambient+u_diffuse_scalar*diffuse)*(1-reflection.a)+reflection.rgb*reflection.a,1);
  if(u_dithering==1)
    out_color.xyz+=dither;
}
