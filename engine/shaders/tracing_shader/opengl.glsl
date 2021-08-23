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

layout(location = 0) out vec4 out_albedo;
layout(location = 1) out vec4 out_normal;
layout(location = 2) out vec4 out_position;
layout(location = 3) out vec4 out_diffuse;
layout(location = 4) out vec4 out_ambient;
layout(location = 5) out vec4 out_debug;


in vec2 v_tex_coords;


uniform sampler3D u_world_tex;
uniform vec3 u_world_tex_size;

uniform sampler2D u_blue_noise;
uniform vec2 u_bn_size;
uniform int u_frame_num;

uniform vec3 u_ambient_color;
uniform int u_num_ao_rays;
uniform float u_ao_ray_length;

uniform int u_soft_shadows;
uniform int u_use_temporal;
uniform int u_lum;


struct Object{
  mat4 transform;
  vec3 tex_size;
  vec3 vox_size;
  vec3 inv_size;
  sampler3D tex;
};
uniform Object u_objects[30];
uniform int u_num_objects;

struct Material{
    vec4 color;
};
uniform Material u_materials[256];

struct Light{
  vec3 position;
  float long_range;
  float short_range;
  vec3 color;
  float radius;
};
uniform int u_num_lights;
uniform Light u_lights[32];

struct Camera{
  mat4 view_projection;
  vec3 origin;
};
uniform Camera u_cam;

struct Ray{
  vec3 origin;
  vec3 dir;
};


struct Intersect{
  bool hit;
  float tmin;
  float tmax;
};
struct Hit{
  bool hit;
  float dist;
  vec3 normal;
  int mat;
};
Intersect intersect(in Ray ray,in vec3 b_min, in vec3 b_max) {

  float tx1 = (b_min.x - ray.origin.x)/ray.dir.x;
  float tx2 = (b_max.x - ray.origin.x)/ray.dir.x;
  

  float tmin = min(tx1, tx2);
  float tmax = max(tx1, tx2);

  float ty1 = (b_min.y - ray.origin.y)/ray.dir.y;
  float ty2 = (b_max.y - ray.origin.y)/ray.dir.y;


  tmin = max(tmin, min(ty1, ty2));
  tmax = min(tmax, max(ty1, ty2));

  float tz1 = (b_min.z - ray.origin.z)/ray.dir.z;
  float tz2 = (b_max.z - ray.origin.z)/ray.dir.z;


  tmin = max(tmin, min(tz1, tz2));
  tmax = min(tmax, max(tz1, tz2));

  return Intersect(tmax >= max(0.0, tmin) && tmin < 10000,tmin, tmax);
};
vec3 getTexCoords(in vec3 pos, in Object obj){
  vec3 tex_coords = pos / (obj.tex_size*obj.vox_size) + 0.5;
  return tex_coords;
}

uint GetVoxel(in sampler3D tex, in vec3 tex_coords){
    return uint(texture(tex,tex_coords)*255)  ;
}


bool algoHit(in Ray ray, in Object obj,in float len)
{


    
    vec3 b_size = obj.tex_size*obj.vox_size;
    vec3 v_size = obj.vox_size;
    vec3 t_size = obj.tex_size;

    vec3 dir = ray.origin;

    vec3 tex_coords = ray.origin/b_size + 0.5;
    





    
    vec3 coords = clamp(floor(tex_coords * t_size), vec3(0), t_size - vec3(1));

    vec3 steps = vec3(sign(ray.dir.x),sign(ray.dir.y),sign(ray.dir.z));


    float outOfX = steps.x < 0? -1:t_size.x;
    float outOfY = steps.y < 0? -1:t_size.y;
    float outOfZ = steps.z < 0? -1:t_size.z;

    vec3 plane_steps = vec3(steps.x<0?0:1,steps.y<0?0:1,steps.z<0?0:1);
    vec3 planes = vec3((((coords + plane_steps)/t_size) - 0.5) * b_size);


    float distX = abs(dir.x - planes.x);
    float distY = abs(dir.y - planes.y);
    float distZ = abs(dir.z - planes.z);






    float tMaxX = abs(distX/ray.dir.x);
    float tMaxY = abs(distY/ray.dir.y);
    float tMaxZ = abs(distZ/ray.dir.z);



    vec3 tDelta = vec3(abs(v_size.x/ray.dir.x), abs(v_size.y/ray.dir.y), abs(v_size.z/ray.dir.z));



    bool first = true;
    float dist = 0;
    while(true){
        uint voxel = GetVoxel(obj.tex,coords*obj.inv_size);
        if(dist>=len) break;
        if( voxel != uint(0)){
          return true;
        }
        if(tMaxX < tMaxY) {
            if(tMaxX < tMaxZ) {
                coords.x += steps.x;
                dist = tMaxX;
                if(abs(coords.x - outOfX)<0.2) break;


                tMaxX= tMaxX + tDelta.x;

            } else {
                coords.z += steps.z;
                dist = tMaxZ;
                if(abs(coords.z - outOfZ)<0.2) break;

                tMaxZ= tMaxZ + tDelta.z;
            }
        } else {
            if(tMaxY < tMaxZ) {
                coords.y += steps.y;
                dist = tMaxY;
                if(abs(coords.y - outOfY)<0.2) break;

                tMaxY= tMaxY + tDelta.y;
            } else {
                coords.z += steps.z;
                dist = tMaxZ;
                if(abs(coords.z - outOfZ)<0.2) break;

                tMaxZ= tMaxZ + tDelta.z;
            }
        }


    }
    return false;
}
Hit algo(in Ray ray, in Object obj)
{
    Hit ray_hit;
    ray_hit.hit = false;
    ray_hit.dist = 0;
    ray_hit.normal = vec3(0,0,0);


    
    vec3 b_size = obj.tex_size*obj.vox_size;
    vec3 v_size = obj.vox_size;
    vec3 t_size = obj.tex_size;

    vec3 dir = ray.origin;

    vec3 tex_coords = (ray.origin/b_size + 0.5);

    





    
    vec3 coords = clamp(floor(tex_coords * t_size), vec3(0), t_size - vec3(1));

    vec3 steps = vec3(sign(ray.dir.x),sign(ray.dir.y),sign(ray.dir.z));


    float outOfX = steps.x < 0? -1:t_size.x;
    float outOfY = steps.y < 0? -1:t_size.y;
    float outOfZ = steps.z < 0? -1:t_size.z;

    vec3 plane_steps = vec3(steps.x<0?0:1,steps.y<0?0:1,steps.z<0?0:1);
    vec3 planes = vec3((((coords + plane_steps)/t_size) - 0.5) * b_size);


    float distX = abs(dir.x - planes.x);
    float distY = abs(dir.y - planes.y);
    float distZ = abs(dir.z - planes.z);






    float tMaxX = abs(distX/ray.dir.x);
    float tMaxY = abs(distY/ray.dir.y);
    float tMaxZ = abs(distZ/ray.dir.z);


    vec3 n_floor = floor(tex_coords+0.00001);
    vec3 n_ceil = ceil(tex_coords-0.00001) - 1;
    ray_hit.normal = n_floor+n_ceil;



    vec3 tDelta = vec3(abs(v_size.x/ray.dir.x), abs(v_size.y/ray.dir.y), abs(v_size.z/ray.dir.z));



    while(true){
        uint voxel = GetVoxel(obj.tex,coords*obj.inv_size);
        if( voxel != uint(0)){
            ray_hit.mat = int(voxel)-1;
            ray_hit.hit = true;

            return ray_hit;
        }
        if(tMaxX < tMaxY) {
            if(tMaxX < tMaxZ) {
                coords.x += steps.x;
                ray_hit.normal = vec3(-steps.x,0,0);
                ray_hit.dist = tMaxX;
                if(abs(coords.x - outOfX)<0.2) break;


                tMaxX= tMaxX + tDelta.x;

            } else {
                coords.z += steps.z;
                ray_hit.normal = vec3(0,0,-steps.z);
                ray_hit.dist = tMaxZ;
                if(abs(coords.z - outOfZ)<0.2) break;

                tMaxZ= tMaxZ + tDelta.z;
            }
        } else {
            if(tMaxY < tMaxZ) {
                coords.y += steps.y;
                ray_hit.normal = vec3(0,-steps.y,0);
                ray_hit.dist = tMaxY;
                if(abs(coords.y - outOfY)<0.2) break;

                tMaxY= tMaxY + tDelta.y;
            } else {
                coords.z += steps.z;
                ray_hit.normal = vec3(0,0,-steps.z);
                ray_hit.dist = tMaxZ;
                if(abs(coords.z - outOfZ)<0.2) break;

                tMaxZ= tMaxZ + tDelta.z;
            }
        }


    }
    return ray_hit;
}

struct FirstPass{
  vec3 albedo;
  vec3 normal;
  bool debug;
  vec3 debug_color;
  float depth;
};

FirstPass firstPass(Ray in_ray){
  FirstPass d;
  d.depth = 100;//TODO: add uniform for this value 
  d.debug = false;
  for(int i=0;i<u_num_objects;++i){
    Object obj = u_objects[i];
    Ray ray = in_ray;
    ray.dir=vec3(inverse(obj.transform)*vec4(ray.dir,0));
    ray.origin=vec3(inverse(obj.transform)*vec4(ray.origin,1));
    vec3 b_min = -obj.tex_size*obj.vox_size/2.0;
    vec3 b_max = obj.tex_size*obj.vox_size/2.0;
    Intersect h = intersect(ray,b_min, b_max);
    if(h.hit){
      float t = max(0.0, h.tmin);// the amount we have to move our ray to enter the box

      if(t > d.depth){ // first depth test
        continue;
      }

      ray.origin+= t*ray.dir;

      /* vec3 tex_coords = (ray.origin/(obj.tex_size*obj.vox_size) + 0.5)*(obj.e_tex - obj.s_tex); */
      /* d.albedo = tex_coords; */
      /* d.depth = t; */
      /* continue; */

      Hit hit = algo(ray,obj);
      if(hit.hit){
        t+=hit.dist;
        if(t > d.depth){ // first depth test
          continue;
        }
        d.depth = t;
        d.albedo = vec3(u_materials[hit.mat].color);
        d.normal = vec3(obj.transform*vec4(hit.normal,0));
      }
    }
  }

  // debug for lights
  for(int i=0;i<u_num_lights;++i){
    Light light = u_lights[i];
    Intersect h = intersect(in_ray,light.position - vec3(light.radius), light.position + vec3(light.radius));
    if(h.hit&&d.depth > h.tmin){
      d.debug_color = light.color;
      d.debug = true;
    }
  }
  return d;
}

struct SecondPass{
  vec3 diffuse;
  vec3 ambient;
  vec3 specular;
};

bool hitObjects(in Ray in_ray, in float len){
  for(int i =0; i <u_num_objects;++i)
  {
    Object obj = u_objects[i];
    Ray ray = in_ray;
    ray.dir=vec3(inverse(obj.transform)*vec4(ray.dir,0));
    ray.origin=vec3(inverse(obj.transform)*vec4(ray.origin,1));
    vec3 b_min = -obj.tex_size*obj.vox_size/2.0;
    vec3 b_max = obj.tex_size*obj.vox_size/2.0;
    Intersect h = intersect(ray,b_min, b_max);
    if(h.hit){
      float t = max(0.0, h.tmin);// the amount we have to move our ray to enter the box
      if(t >=len) continue;
      ray.origin+= t*ray.dir;
      vec3 tex_coords = getTexCoords(ray.origin, obj);
      if(algoHit(ray,obj,len-t)){
        return true;
      }
    }
  }
  return false;
}

float IRATIONAL_0 = 1.61803398875;
float IRATIONAL_1 = 1.32471795724;
float IRATIONAL_2 = 1.220744084605; 
float getLum(in vec3 col){
  return (0.2126*col.b + 0.7152*col.b + 0.0722*col.b);
}
SecondPass secondPass(in vec3 position, in vec3 normal, in vec3 albedo){
  SecondPass d;
  d.diffuse = vec3(0);
  d.ambient = vec3(0);
  d.specular = vec3(0);

  // get random vector with blue noise
  vec3 blue = texture(u_blue_noise,gl_FragCoord.xy/u_bn_size).xyz;
  int j = u_frame_num*u_use_temporal+1;
  vec3 popo = vec3(IRATIONAL_0*(j%97), IRATIONAL_1*(j%137), IRATIONAL_2*(j%67));
  vec3 noise = mod(blue+popo,1.0)*2 - 1;

  float in_shadow = 1;
  for(int i=0;i<u_num_lights;++i){
    Light light = u_lights[i];
    
    vec3 l_dir = light.position-position+ light.radius*noise*u_soft_shadows;

    float diff = max(0.0,dot(normal,normalize(l_dir)));
    float len = length(l_dir);
    float range_mul = 1;
    if(len> light.short_range)
      range_mul = min((light.long_range-length(l_dir)-light.short_range)/(light.long_range-light.short_range),1.0); // negative means 1
    if(len > light.long_range){
      continue;
    }


    //specular
    vec3 view_dir = normalize(u_cam.origin-position);
    vec3 reflect_dir = reflect(-normalize(l_dir),normal);
    float spec = pow(max(dot(view_dir,reflect_dir),0.0),32);

    Ray ray;
    ray.dir = normalize(l_dir);
    ray.origin = position+ray.dir*0.001;
    if(!hitObjects(ray, len)){
      d.diffuse += (light.color+light.color*spec)*diff*range_mul;
      in_shadow = 0;
    }

  }
  
  /* if(!in_shadow)// ambient color from the light */
  if(u_lum==1){
    float lum = getLum(albedo*(0.5+0.5*d.diffuse));
    d.ambient = u_ambient_color*lum/float(u_num_ao_rays);
  }

  for(int i=0;i<u_num_ao_rays;++i){
    int j = u_frame_num*u_num_ao_rays*u_use_temporal+i+1;
    vec3 popo = vec3(IRATIONAL_0*(j%97), IRATIONAL_1*(j%137), IRATIONAL_2*(j%67));
    vec3 noise = mod(blue+popo,1.0);

    noise.gb = noise.gb*2 - 1;

    vec3 n = normal;
    vec3 r1 = normalize(cross(normal,normal.xxx+vec3(0.1,0,0)));
    vec3 r2 = normalize(cross(normal,r1));
    vec3 dir = n*noise.r + r1 * noise.g + r2*noise.b;

    Ray ray;
    ray.origin = position +0.01*normal;
    ray.dir = normalize(dir);
    if(!hitObjects(ray,u_ao_ray_length)){
      d.ambient += (u_ambient_color)/float(u_num_ao_rays);//TODO: use u_ambient_color for outdoor scenes
      /* d.ambient += (dir*0.5+1)/float(u_num_ao_rays);//TODO: use u_ambient_color for outdoor scenes */
    }
  }

  return d;
}

void main()
{

    Ray ray;
    ray.origin = u_cam.origin; 

    vec4 wp = inverse(u_cam.view_projection)*vec4(v_tex_coords*2-1,0.0,1);
    wp/=wp.w;
    ray.dir = normalize(vec3(wp) - ray.origin);


    

    FirstPass fp = firstPass(ray);
    if(fp.debug){
      out_debug = vec4(fp.debug_color,1);
      return;
    }
    if(fp.depth >99){//TODO: also replace the number with uniform here
      discard;
    }
    out_debug = vec4(0);


    vec3 pos = ray.origin+ray.dir*fp.depth;
    SecondPass sp = secondPass(pos, fp.normal, fp.albedo);

    out_albedo = vec4(fp.albedo,1);
    out_normal = vec4(fp.normal,1);
    out_diffuse = vec4(sp.diffuse,1);
    out_ambient = vec4(sp.ambient,1);
    /* out_diffuse = vec4(sp.ambient,1); */
    out_position = vec4(pos,1);

}
