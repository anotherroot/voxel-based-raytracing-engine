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
uniform sampler2D u_light;




void main()
{
  vec3 light = texture(u_light, v_tex_coords).xyz;
  vec3 albedo = texture(u_albedo, v_tex_coords).xyz;
  out_color = vec4(light*2*albedo,1);
  /* out_color = vec4(vec3(u_ambient_count/60.0),1); */
}
