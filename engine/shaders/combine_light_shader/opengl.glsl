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

uniform sampler2D u_diffuse;
uniform sampler2D u_ambient;

uniform float u_diffuse_scalar;
uniform float u_ambient_scalar;

void main()
{

  vec3 diffuse = texture(u_diffuse, v_tex_coords).xyz;
  vec3 ambient = texture(u_ambient, v_tex_coords).xyz;

  out_color = vec4(u_ambient_scalar*ambient + u_diffuse_scalar*diffuse,1);
}
