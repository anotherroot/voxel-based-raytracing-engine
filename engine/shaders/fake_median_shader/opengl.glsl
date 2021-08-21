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


 

float dx = 1.0/u_window_size.x;
float dy = 1.0/u_window_size.y;


struct Node{
  vec3 val;
  float lval;
};

void main()
{
  vec3 ret = vec3(0);


  int radius = u_filter_radius;
  float sum = 0;
  vec3 c_pos = texture(u_position,v_tex_coords).xyz; 
  vec3 c_normal = texture(u_normal,v_tex_coords).xyz; 
  Node nodes[9];
  int num = 0;
  for(int i=-1;i<=1;++i){
    for(int j=-1;j<=1;++j){

      vec3 val = texture(u_texture,v_tex_coords+vec2(i*dx,j*dy)).xyz; 
      vec3 normal = texture(u_normal,v_tex_coords+vec2(i*dx,j*dy)).xyz; 
      vec3 pos = texture(u_position,v_tex_coords+vec2(i*dx,j*dy)).xyz; 
      if(length(normal-c_normal)>0.1||length(c_pos-pos)>u_pos_th){
        continue;
      }

      float lval = (0.2126*val.r + 0.7152*val.g + 0.0722*val.b);
      nodes[num].val = val;
      nodes[num].lval = lval;
      num++;
    }
  }


  
  for(int i=0;i<num;++i){
    for(int j=1;j<num;++j){
      if(nodes[j-1].lval >nodes[j].lval){
        Node t = nodes[j];
        nodes[j] = nodes[j-1];
        nodes[j-1] = t;
      }
    }
  }



  out_color = vec4(nodes[num/2].val,1);
}
