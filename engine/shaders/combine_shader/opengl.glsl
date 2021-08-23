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
uniform sampler2D u_albedo;
uniform sampler2D u_light;
uniform sampler2D u_debug;
uniform sampler2D u_dither;
uniform int u_dithering;



void main()
{
  vec3 albedo = texture(u_albedo, v_tex_coords).xyz;
  vec3 light = texture(u_light, v_tex_coords).xyz;
  vec4 debug = texture(u_debug, v_tex_coords);
  vec3 dither = vec3(texture(u_dither, gl_FragCoord.xy/16.0).r / 32.0 -(1.0/64.0));
  if(debug.a>0.5){
  out_color = debug;
    return;
  }
  out_color = vec4(light*albedo,1);
  if(u_dithering==1)
    out_color.xyz+=dither;
  /* out_color = vec4(dither*100,1); */
  /* out_color = vec4(vec3(u_ambient_count/60.0),1); */
}
