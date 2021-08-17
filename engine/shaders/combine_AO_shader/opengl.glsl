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

uniform sampler2D u_AO[10];
uniform int u_ignore_old;
uniform int u_num;



void main()
{
  vec3 color = vec3(0);
  int n = u_num;
  /* out_color= vec4(vec3(n/10.0),1); */
  /* return; */
  for(int i=0;i<n;++i){
    color+=texture(u_AO[i],v_tex_coords).xyz/float(n); 
  }
  out_color = vec4(color,1);
}
