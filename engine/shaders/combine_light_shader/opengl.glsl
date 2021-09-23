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

layout(location = 0) out vec4 out_diffuse;
layout(location = 1) out vec4 out_ambient;
layout(location = 2) out vec4 out_reflection;


in vec2 v_tex_coords;

uniform sampler2D u_diffuse;
uniform sampler2D u_ambient;
uniform sampler2D u_reflection;

uniform sampler2D u_old_ambient;
uniform sampler2D u_old_diffuse;
uniform sampler2D u_old_reflection;

uniform float u_inv_num;
uniform float u_inv_old;

void main()
{
  float inv_old = u_inv_old;
  float inv_num = u_inv_num;

  vec3 old_diffuse = texture(u_old_diffuse,v_tex_coords).xyz;
  vec3 diffuse = texture(u_diffuse, v_tex_coords).xyz;
  out_diffuse = vec4((old_diffuse*(1-inv_num)+diffuse*inv_num),1);

  vec3 old_ambient = texture(u_old_ambient,v_tex_coords).xyz*inv_old;
  vec3 ambient = texture(u_ambient, v_tex_coords).xyz;
  out_ambient = vec4((old_ambient+ambient)*inv_num,1);

  vec3 old_reflection = texture(u_old_reflection,v_tex_coords).xyz*inv_old;
  vec4 reflection = texture(u_reflection, v_tex_coords);
  out_reflection = vec4((old_reflection+reflection.xyz)*inv_num,reflection.a);
}
