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
uniform sampler2D u_texture;
uniform sampler2D u_normal;
uniform sampler2D u_position;
uniform int u_filter_radius;
uniform float u_pos_th;
uniform int u_filter_cross;

 

float dx = 1.0/u_window_size.x;
float dy = 1.0/u_window_size.y;




void main()
{
  out_color = texture(u_texture, v_tex_coords);

}
