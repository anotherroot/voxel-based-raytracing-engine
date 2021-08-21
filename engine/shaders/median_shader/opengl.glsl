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
// Input texture
uniform vec2 u_window_size;
uniform sampler2D u_texture;
uniform sampler2D u_normal;
uniform sampler2D u_position;
uniform int u_filter_radius;
uniform float u_pos_th;

in vec2 v_tex_coords;
// Change these 2 defines to change precision
#define vec vec3
#define toVec(x) x.rgb

#define s2(a, b)				temp = a; a = min(a, b); b = max(temp, b);
#define t2(a, b)				s2(v[a], v[b]);
#define t24(a, b, c, d, e, f, g, h)			t2(a, b); t2(c, d); t2(e, f); t2(g, h); 
#define t25(a, b, c, d, e, f, g, h, i, j)		t24(a, b, c, d, e, f, g, h); t2(i, j);


float dx = 1.0/u_window_size.x;
float dy = 1.0/u_window_size.y;
void main() {

  vec v[25];

  vec3 c_pos = texture(u_position,v_tex_coords).xyz; 
  vec3 c_normal = texture(u_normal,v_tex_coords).xyz; 
  // Add the pixels which make up our window to the pixel array.
  int bs=0;
  for(int dX = -2; dX <= 2; ++dX) {
    for(int dY = -2; dY <= 2; ++dY) {		
      vec2 offset = vec2(float(dX)*dx, float(dY)*dy);
		    
      vec3 normal = texture(u_normal,v_tex_coords+offset).xyz; 
      vec3 pos = texture(u_position,v_tex_coords+offset).xyz; 

      if(length(normal-c_normal)>0.1||length(c_pos-pos)>u_pos_th){
        v[(dX + 2) * 5 + (dY + 2)] = vec3(bs);
        bs=(bs+1)%2;
        continue;
      }
      // If a pixel in the window is located at (x+dX, y+dY), put it at index (dX + R)(2R + 1) + (dY + R) of the
      // pixel array. This will fill the pixel array, with the top left pixel of the window at pixel[0] and the
      // bottom right pixel of the window at pixel[N-1].
      v[(dX + 2) * 5 + (dY + 2)] = toVec(texture(u_texture, v_tex_coords + offset));
    }
  }

  vec temp;

  t25(0, 1,			3, 4,		2, 4,		2, 3,		6, 7);
  t25(5, 7,			5, 6,		9, 7,		1, 7,		1, 4);
  t25(12, 13,		11, 13,		11, 12,		15, 16,		14, 16);
  t25(14, 15,		18, 19,		17, 19,		17, 18,		21, 22);
  t25(20, 22,		20, 21,		23, 24,		2, 5,		3, 6);
  t25(0, 6,			0, 3,		4, 7,		1, 7,		1, 4);
  t25(11, 14,		8, 14,		8, 11,		12, 15,		9, 15);
  t25(9, 12,		13, 16,		10, 16,		10, 13,		20, 23);
  t25(17, 23,		17, 20,		21, 24,		18, 24,		18, 21);
  t25(19, 22,		8, 17,		9, 18,		0, 18,		0, 9);
  t25(10, 19,		1, 19,		1, 10,		11, 20,		2, 20);
  t25(2, 11,		12, 21,		3, 21,		3, 12,		13, 22);
  t25(4, 22,		4, 13,		14, 23,		5, 23,		5, 14);
  t25(15, 24,		6, 24,		6, 15,		7, 16,		7, 19);
  t25(3, 11,		5, 17,		11, 17,		9, 17,		4, 10);
  t25(6, 12,		7, 14,		4, 6,		4, 7,		12, 14);
  t25(10, 14,		6, 7,		10, 12,		6, 10,		6, 17);
  t25(12, 17,		7, 17,		7, 10,		12, 18,		7, 12);
  t24(10, 18,		12, 20,		10, 20,		10, 12);
  /* toVec(gl_FragColor) = v[12]; */
  out_color = vec4(v[12],1);  

}




//float lessThan(float a, float b) {
//	return a < b;
//}

// Swap so that a < b.  If b > a, then delta is > 0.  In this case, test = 0 and no swap occurs
//#define s2(a, b)				delta = b - a; test = vec(lessThan(delta, ZERO)); a += delta * test;  b -= delta * test;
//#define s2DropLo(a, b)			delta = b - a; test = vec(lessThan(delta, ZERO)); b -= delta * test;
//#define s2DropHi(a, b)			delta = b - a; test = vec(lessThan(delta, ZERO)); a += delta * test;

/*
  // Optional :Expand out the last step to dump the result directly into gl_FragColor
  /* mx3(v[3], v[4], v[8]); */
  /* delta = v[4] - v[3]; */
  /* test = vec3(lessThan(delta, ZERO)) */
  /* gl_FragColor = b - delta * test;*/

