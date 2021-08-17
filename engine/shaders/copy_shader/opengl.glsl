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

uniform sampler2D u_texture;


void main()
{
    out_color = texture(u_texture, v_tex_coords);
}
