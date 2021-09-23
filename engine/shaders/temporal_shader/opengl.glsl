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
/* layout(location = 1) out vec4 out_ambient; */
/* layout(location = 2) out vec4 out_reflection; */

in vec2 v_tex_coords;

uniform sampler2D u_old_diffuse;
/* uniform sampler2D u_old_ambient; */
/* uniform sampler2D u_old_reflection; */


uniform sampler2D u_normal;
uniform sampler2D u_position;
uniform sampler2D u_diffuse;
/* uniform sampler2D u_ambient; */
/* uniform sampler2D u_reflection; */

uniform float u_inv_num;
uniform float u_inv_old;

void main()
{

  /* vec3 old_normal = texture(u_old_normal, v_tex_coords).xyz*u_inv_old; */
  /* vec3 old_position = texture(u_old_position, v_tex_coords).xyz*u_inv_old; */
  /* vec3 old_diffuse = texture(u_old_diffuse, v_tex_coords).xyz*u_inv_old; */
  /* vec3 old_ambient = texture(u_old_ambient, v_tex_coords).xyz*u_inv_old; */
  /* vec3 old_reflection = texture(u_old_reflection, v_tex_coords).xyz*u_inv_old; */

  /* vec3 normal = texture(u_normal, v_tex_coords).xyz; */
  /* vec3 position = texture(u_position, v_tex_coords).xyz; */
  vec3 diffuse = texture(u_diffuse, v_tex_coords).xyz;
  /* vec3 ambient = texture(u_ambient, v_tex_coords).xyz; */
  /* vec3 reflection = texture(u_reflection, v_tex_coords).xyz; */

  out_diffuse = vec4(diffuse,1);
  /* out_ambient = vec4(ambient,1); */
  /* out_reflection = vec4(reflection,1); */
  /* out_diffuse = vec4(1,0,0,1); */
}
