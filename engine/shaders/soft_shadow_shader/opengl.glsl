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


vec3 ret = vec3(0);
  vec3 c_pos = texture(u_position,v_tex_coords).xyz; 
  vec3 c_normal = texture(u_normal,v_tex_coords).xyz; 
  int radius = u_filter_radius;
int AddPixel(int i, int j){
  vec2 tex_coords = v_tex_coords+vec2(i*dx,j*dy);
  vec3 pos = texture(u_position,tex_coords).xyz; 
  vec3 normal = texture(u_normal,tex_coords).xyz; 
  if(length(c_normal-normal)>0.01||length(pos-c_pos)>u_pos_th)
    return 0;
  ret+=texture(u_texture, tex_coords).xyz;
  return 1;
}

void main()
{

  /* c_pos = texture(u_position,v_tex_coords).xyz; */ 
  /* c_normal = texture(u_normal,v_tex_coords).xyz; */ 
  /* radius = u_filter_radius; */

  float sum = 0;
  if(u_filter_cross==0){ 
    for(int i=-radius;i<=radius;++i){
      for(int j=-radius;j<=radius;++j){
        sum+=AddPixel(i,j); 
      }
    }
  }
  else if(u_filter_cross == 1){

    /* for(int i=-radius;i<=radius;++i){ */
    /*   sum+=AddPixel(i,i); */ 
    /*   if(i!=0) */
    /*     sum+=AddPixel(-i,i); */ 
    /* } */

    for(int i=-radius;i<=radius;++i){
      sum+=AddPixel(i,0); 
    }
  } 
  else{ 

    /* for(int i=-radius;i<=radius;++i){ */
    /*   sum+=AddPixel(i,0); */ 
    /* } */
    /* for(int j=-radius;j<=radius;++j){ */
    /*   if(j==0)continue; */
    /*   sum+=AddPixel(0,j); */ 
    /* } */

    for(int i=-radius;i<=radius;++i){
      sum+=AddPixel(0,i); 
    }
  }
  out_color = vec4(ret/sum,1);
}
