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
uniform sampler2D u_direct;
uniform sampler2D u_ambient;

uniform int u_show_shadow_caster; 



void main()
{
  vec3 direct = texture(u_direct, v_tex_coords).xyz;
  vec3 ambient = texture(u_ambient, v_tex_coords).xyz;
  vec3 albedo = texture(u_albedo, v_tex_coords).xyz;
  out_color = vec4((0.5*direct+0.5*ambient)*albedo,1);
}
