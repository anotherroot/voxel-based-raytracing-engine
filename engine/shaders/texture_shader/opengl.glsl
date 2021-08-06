#type vertex
#version 330 core
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_tex_coords;

out vec2 v_tex_coords;

void main()
{
    v_tex_coords = a_tex_coords;
    gl_Position =  vec4(a_position,1);
}

#type fragment
#version 330 core
layout(location = 0) out vec4 out_color;



uniform sampler2D u_texture;


in vec2 v_tex_coords;







void main()
{
    /* vec3 blue = texture(u_blue_noise,v_tex_coords).xyz; */
    /* out_color = vec4(v_tex_coords,1,1); */
  out_color = vec4(1,0,0,1);
    /* discard; */
}
