#type vertex
#version 300 es

in mediump vec3 a_position;
in mediump vec2 a_tex_coord;
in mediump float a_tex_index;
in lowp vec4 a_color;

out mediump vec3 v_frag_position;
out lowp vec4 v_color;
out mediump vec2 v_tex_coord;
out mediump float v_tex_index;

uniform mat4 u_view_projection;


void main()
{
    gl_Position = u_view_projection*vec4(a_position,1);
    v_tex_coord = a_tex_coord;
    v_frag_position = a_position;
    v_tex_index = a_tex_index;
    v_color = a_color;
}

#type fragment
#version 300 es

out lowp vec4 color;

in lowp vec4 v_color;
in mediump vec2 v_tex_coord;
in mediump float v_tex_index;

const int NUM_TEXTURES = 16;

uniform sampler2D u_textures[16];

void main()
{
    lowp vec4 my_color;
    switch(int(v_tex_index)){
        case 0: my_color = texture(u_textures[0], v_tex_coord) ; break;
        case 1: my_color = texture(u_textures[1], v_tex_coord); break;
        case 2: my_color = texture(u_textures[2], v_tex_coord) ; break;
        case 3: my_color = texture(u_textures[3], v_tex_coord); break;
        case 4: my_color = texture(u_textures[4], v_tex_coord) ; break;
        case 5: my_color = texture(u_textures[5], v_tex_coord); break;
        case 6: my_color = texture(u_textures[6], v_tex_coord) ; break;
        case 7: my_color = texture(u_textures[7], v_tex_coord); break;
        case 8: my_color = texture(u_textures[8], v_tex_coord) ; break;
        case 9: my_color = texture(u_textures[9], v_tex_coord); break;
        case 10: my_color = texture(u_textures[10], v_tex_coord) ; break;
        case 11: my_color = texture(u_textures[11], v_tex_coord); break;
        case 12: my_color = texture(u_textures[12], v_tex_coord) ; break;
        case 13: my_color = texture(u_textures[13], v_tex_coord); break;
        case 14: my_color = texture(u_textures[14], v_tex_coord) ; break;
        case 15: my_color = texture(u_textures[15], v_tex_coord) ; break;
        default: my_color = texture(u_textures[0], v_tex_coord);
    }
    color=my_color*v_color;
}
