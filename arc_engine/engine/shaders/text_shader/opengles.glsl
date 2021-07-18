#type vertex
#version 330 core

layout (location = 0) in vec2 a_position;
layout (location = 1) in vec2 a_tex_coord;
layout (location = 2) in float a_tex_index;
out vec2 v_tex_coord;
out float v_tex_index;
uniform mat4 u_projection;

void main()
{
    gl_Position = u_projection * vec4(a_position, 0.0, 1.0);
    v_tex_coord = a_tex_coord;
    v_tex_index = a_tex_index;
}

    #type fragment
#version 330 core

in vec2 v_tex_coord;
in float v_tex_index;
out vec4 color;
const int NUM_TEXTURES = 16;

uniform sampler2D u_textures[16];
uniform vec4 u_color;

void main()
{
    vec4 sampled;
    switch(int(v_tex_index)){
        case 0: sampled = vec4(1.0, 1.0, 1.0, texture(u_textures[0], v_tex_coord).r) ; break;
        case 1: sampled = vec4(1.0, 1.0, 1.0, texture(u_textures[1], v_tex_coord).r) ; break;
        case 2: sampled = vec4(1.0, 1.0, 1.0, texture(u_textures[2], v_tex_coord).r) ; break;
        case 3: sampled = vec4(1.0, 1.0, 1.0, texture(u_textures[3], v_tex_coord).r) ; break;
        case 4: sampled = vec4(1.0, 1.0, 1.0, texture(u_textures[4], v_tex_coord).r) ; break;
        case 5: sampled = vec4(1.0, 1.0, 1.0, texture(u_textures[5], v_tex_coord).r) ; break;
        case 6: sampled = vec4(1.0, 1.0, 1.0, texture(u_textures[6], v_tex_coord).r) ; break;
        case 7: sampled = vec4(1.0, 1.0, 1.0, texture(u_textures[7], v_tex_coord).r) ; break;
        case 8: sampled = vec4(1.0, 1.0, 1.0, texture(u_textures[8], v_tex_coord).r) ; break;
        case 9: sampled = vec4(1.0, 1.0, 1.0, texture(u_textures[9], v_tex_coord).r) ; break;
        case 10: sampled = vec4(1.0, 1.0, 1.0, texture(u_textures[10], v_tex_coord).r) ; break;
        case 11: sampled = vec4(1.0, 1.0, 1.0, texture(u_textures[11], v_tex_coord).r) ; break;
        case 12: sampled = vec4(1.0, 1.0, 1.0, texture(u_textures[12], v_tex_coord).r) ; break;
        case 13: sampled = vec4(1.0, 1.0, 1.0, texture(u_textures[13], v_tex_coord).r) ; break;
        case 14: sampled = vec4(1.0, 1.0, 1.0, texture(u_textures[14], v_tex_coord).r) ; break;
        case 15: sampled = vec4(1.0, 1.0, 1.0, texture(u_textures[15], v_tex_coord).r) ; break;
        default: sampled = vec4(1.0, 1.0, 1.0, texture(u_textures[0], v_tex_coord).r) ; break;
    }

//    vec4 my_color;
//    switch(int(v_tex_index)){
//        case 0: my_color = texture(u_textures[0], v_tex_coord) ; break;
//        case 1: my_color = texture(u_textures[1], v_tex_coord); break;
//        case 2: my_color = texture(u_textures[2], v_tex_coord) ; break;
//        case 3: my_color = texture(u_textures[3], v_tex_coord); break;
//        case 4: my_color = texture(u_textures[4], v_tex_coord) ; break;
//        case 5: my_color = texture(u_textures[5], v_tex_coord); break;
//        case 6: my_color = texture(u_textures[6], v_tex_coord) ; break;
//        case 7: my_color = texture(u_textures[7], v_tex_coord); break;
//        case 8: my_color = texture(u_textures[8], v_tex_coord) ; break;
//        case 9: my_color = texture(u_textures[9], v_tex_coord); break;
//        case 10: my_color = texture(u_textures[10], v_tex_coord) ; break;
//        case 11: my_color = texture(u_textures[11], v_tex_coord); break;
//        case 12: my_color = texture(u_textures[12], v_tex_coord) ; break;
//        case 13: my_color = texture(u_textures[13], v_tex_coord); break;
//        case 14: my_color = texture(u_textures[14], v_tex_coord) ; break;
//        case 15: my_color = texture(u_textures[15], v_tex_coord) ; break;
//        default: my_color = texture(u_textures[0], v_tex_coord);
//    }

    //color = u_color * sampled;
    color = sampled;
    //color = vec4(v_tex_coord.xy,1,1);
}
