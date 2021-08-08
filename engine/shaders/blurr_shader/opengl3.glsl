#type vertex
#version 130

in vec3 a_position;
in vec4 a_color;

out vec3 v_frag_position;
out vec4 v_color;

uniform mat4 u_view_projection;


void main()
{
    gl_Position = u_view_projection*vec4(a_position,1);
    v_frag_position = a_position;
    v_color = a_color;
}

#type fragment
#version 130

out vec4 color;
in vec4 v_color;



void main()
{

    color=v_color;



}
