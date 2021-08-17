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
uniform float u_q;

const float gauss[25] = {
  1.0, 4.0,   6.0,  4.0,    1.0,
  4.0, 16.0,  24.0, 16.0,   4.0,
  6.0, 24.0,  36.0, 24.0,   6.0,
  4.0, 16.0,  24.0, 16.0,   4.0,
  1.0, 4.0,   6.0,  4.0,    1.0};
 

float dx = 1.0/u_window_size.x;
float dy = 1.0/u_window_size.y;

vec2 offsets[25]= { 
  vec2(-2*dx,-2*dy), vec2(-1*dx,-2*dy), vec2(+0*dx,-2*dy), vec2(+1*dx,-2*dy), vec2(+2*dx,-2*dy),
  vec2(-2*dx,-1*dy), vec2(-1*dx,-1*dy), vec2(+0*dx,-1*dy), vec2(+1*dx,-1*dy), vec2(+2*dx,-1*dy),
  vec2(-2*dx,+0*dy), vec2(-1*dx,+0*dy), vec2(+0*dx,+0*dy), vec2(+1*dx,+0*dy), vec2(+2*dx,+0*dy),
  vec2(-2*dx,+1*dy), vec2(-1*dx,+1*dy), vec2(+0*dx,+1*dy), vec2(+1*dx,+1*dy), vec2(+2*dx,+1*dy),
  vec2(-2*dx,+2*dy), vec2(-1*dx,+2*dy), vec2(+0*dx,+2*dy), vec2(+1*dx,+2*dy), vec2(+2*dx,+2*dy)};

void main()
{
  vec3 ret = vec3(0);
  float val = texture(u_texture,v_tex_coords).x;
  float w = 0;
  for(int i=0;i<25;++i){
    vec3 v =texture(u_texture, v_tex_coords+offsets[i]).xyz; 
    if(abs(v.x-val)<=u_q){
      ret+=v*gauss[i];
      w+=gauss[i];
    }
  }
  out_color = vec4(ret/w,1);
}
