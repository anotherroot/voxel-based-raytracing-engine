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
layout(location = 5) out vec4 out_reflection;
layout(location = 6) out vec4 out_comp;


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
uniform int u_reflection_depth;

uniform vec3 u_background_color;




struct Object{
  mat4 transform;
  vec3 tex_size;
  vec3 vox_size;
  vec3 inv_size;
  int moving;
  int tex;
};

uniform Object u_objects[300];
uniform int u_num_objects;
uniform sampler3D u_textures[10];
uniform int u_traversal_algo;

struct Material{
  vec3 color;
  float reflection;
  float roughness;
};
uniform Material u_materials[256];

struct Light{
  vec3 position;
  float range;
  vec3 color;
  float radius;
  int moving;
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

struct AABB{
  vec3 tmin;
  vec3 tmax;
};


#define RAY_RANGE 100

AABB aabb(in Ray ray,in vec3 b_min, in vec3 b_max) {
  AABB o;

  float tx1 = (b_min.x - ray.origin.x)/ray.dir.x;
  float tx2 = (b_max.x - ray.origin.x)/ray.dir.x;

  o.tmin.x = min(tx1, tx2);
  o.tmax.x = max(tx1, tx2);

  float ty1 = (b_min.y - ray.origin.y)/ray.dir.y;
  float ty2 = (b_max.y - ray.origin.y)/ray.dir.y;

  o.tmin.y = min(ty1, ty2);
  o.tmax.y = max(ty1, ty2);

  float tz1 = (b_min.z - ray.origin.z)/ray.dir.z;
  float tz2 = (b_max.z - ray.origin.z)/ray.dir.z;

  o.tmin.z = min(tz1, tz2);
  o.tmax.z = max(tz1, tz2);

  return o;
};

ivec3 GetVoxel(in sampler3D tex, in vec3 tex_coords){
    return ivec3(texture(tex,tex_coords).rgb*vec3(255));
}

struct Hit{
  bool hit;
  float dist;
  vec3 normal;
  int mat;
};

Hit traverseFastDDA(in vec3 p0, in vec3 p1, in sampler3D tex,in Object obj, in vec3 tmin)
{
  Hit hit;
  hit.hit = false;
  hit.dist = 0;
  hit.normal = vec3(0);
  vec3 rd = p1 - p0; 
  vec3 p = clamp(floor(p0),vec3(0.0), obj.tex_size-1);
  vec3 rd_inv = 1.0 / rd;
  vec3 stp = sign(rd);
	vec3 delta = min(rd_inv * stp, 1.0);
  vec3 t_max = abs((p + max(stp, vec3(0.0)) - p0) * rd_inv);
  hit.normal = step(tmin.zxy,tmin.xyz) * step(tmin.yzx,tmin.xyz)*stp;
  for (int i = 0; i < 128; ++i) {
    ivec3 voxel = GetVoxel(tex,p*obj.inv_size);
    if( voxel.r != int(0)){
        hit.mat = voxel.r-1;
        hit.hit = true;
        out_comp.rg+=vec2(i,-i);
        break;
    }
    vec3 tmp_max = t_max;//+delta * vec3(voxel.b-1.0);
    hit.dist = min(min(tmp_max.x,tmp_max.y), tmp_max.z);
    if (hit.dist > 0.999){ 
      out_comp.rg+=vec2(i,-i);
      break;
    }
    hit.normal = step(tmp_max.xyz,tmp_max.zxy) * step(tmp_max.xyz,tmp_max.yzx)*stp;
    vec3 p2 = floor(p0+ (hit.dist) * rd+0.1*hit.normal) ;
    vec3 cmp = floor(abs(p2-p));
    t_max += delta * cmp;
    p += stp * cmp ;
  }  
  hit.dist*=length(rd);
  hit.normal *=-1;
  return hit;
}

Hit traverse(in Ray ray, in Object obj, in sampler3D tex, in float tmax, in vec3 tmin){
  vec3 b_size = obj.tex_size;
  vec3 p0 = ray.origin/obj.vox_size+b_size*0.5;
  vec3 p1 = (ray.origin+ray.dir*tmax)/obj.vox_size+b_size*0.5;
  Hit hit =traverseFastDDA(p0,p1,tex,obj,tmin); 
  hit.dist*=obj.vox_size.r;
  /* out_comp.rg+=vec2(1,-1); */
  return hit;
}


Hit intersectObject(in Ray ray, in int i, in float depth){
    Hit hit;
    hit.hit = false;
    Object obj = u_objects[i];
    // ray rebasing
    ray.dir = vec3(inverse(obj.transform) * vec4(ray.dir,0));
    ray.origin = vec3(inverse(obj.transform) * vec4(ray.origin,1));

    // intersection AABB
    vec3 b_min = -obj.tex_size*obj.vox_size/2.0;
    vec3 b_max = obj.tex_size*obj.vox_size/2.0;
    AABB ab = aabb(ray,b_min, b_max);
    float tmax =min(ab.tmax.x,min(ab.tmax.z,ab.tmax.y)); 
    float tmin =max(ab.tmin.x,max(ab.tmin.z,ab.tmin.y)); 
    float t = max(0.0,tmin);  

    if( tmax>t && t < depth){
      ray.origin += t*ray.dir; //  move ray to the box
      hit = traverse(ray,obj, u_textures[obj.tex],tmax-t,ab.tmin);
      hit.dist += t;
      hit.hit = hit.hit && hit.dist<depth;
      hit.normal = vec3(obj.transform*vec4(hit.normal,0));
    }
    return hit;
}

struct Intersect{
  int mat;
  vec3 normal;
  bool light;
  vec3 light_color;
  float depth;
};

Intersect intersect(Ray in_ray){
  Intersect d;
  d.depth = RAY_RANGE+1;
  d.light = false;

  // Objects
  for(int i=0;i<u_num_objects;++i){
    Hit hit = intersectObject(in_ray,i,d.depth);
    if(hit.hit){
      d.depth = hit.dist;
      d.mat = hit.mat;
      d.normal = hit.normal;
    }
  }

  // Lights
  for(int i=0;i<u_num_lights;++i){
    Light light = u_lights[i];
    AABB ab = aabb(in_ray,light.position - vec3(light.radius), light.position + vec3(light.radius));
    /* out_comp.rg+=vec2(i,-i); */
    float tmax =min(ab.tmax.x,min(ab.tmax.z,ab.tmax.y)); 
    float tmin =max(ab.tmin.x,max(ab.tmin.z,ab.tmin.y)); 
    float t = max(0.0,tmin);  

    if(tmax > t&&d.depth > t){
      d.light_color = light.color;
      d.light = true;
      d.depth = t;
    }
  }

  return d;
}

bool intersectFast(in Ray in_ray, in float len){
  for(int i =0; i <u_num_objects;++i)
  {
    if(intersectObject(in_ray,i,len).hit){
      return true;
    }
  }
  return false;
}



float IRATIONAL_0 = 1.61803398875;
float IRATIONAL_1 = 1.32471795724;
float IRATIONAL_2 = 1.220744084605; 

struct LightData{
  vec3 diffuse;
  vec3 ambient;
};

uniform float u_ambient_start;
LightData getLight(vec3 pos, vec3 normal, Material mat){
  LightData ld;
  ld.diffuse = vec3(0);
  ld.ambient = vec3(0);

  // blue noise sampling
  vec3 blue = texture(u_blue_noise,gl_FragCoord.xy/u_bn_size).xyz;
  int j = u_frame_num+1;
  vec3 popo = vec3(IRATIONAL_0*(j%97), IRATIONAL_1*(j%137), IRATIONAL_2*(j%67));
  vec3 noise = mod(blue+popo,1.0)*2 - 1;

  for(int i=0;i<u_num_lights;++i){
    Light light = u_lights[i];
    
    vec3 l_dir = light.position-pos+ light.radius*noise*u_soft_shadows;
    Ray ray = Ray(pos, normalize(l_dir));

    float diff = max(0.0,dot(normal,normalize(l_dir)));
    vec3 tmin = aabb(ray,light.position-vec3(light.radius),light.position+vec3(light.radius)).tmin;
    float len = max(tmin.x, max(tmin.z, tmin.y));
    float range_mul = 1;
    if(len> light.range)
      range_mul = min((light.range-len)/light.range,1.0); // negative means 1
    if(len > light.range){
      continue;
    }


    ray.origin = pos+ray.dir*0.0001;
    if(!intersectFast(ray, len)){
      ld.diffuse += (light.color)*diff*range_mul;
    }

  }
  
  
  vec3 n = normal;
  vec3 r1 = normalize(cross(normal,normal.xxx+vec3(0.1,0,0)));
  vec3 r2 = normalize(cross(normal,r1));
  for(int i=0;i<u_num_ao_rays;++i){
    int j = u_frame_num*u_num_ao_rays+i+1;
    vec3 popo = vec3(IRATIONAL_0*(j%97), IRATIONAL_1*(j%137), IRATIONAL_2*(j%167));
    vec3 noise = mod(blue+popo,1.0);
    noise.gb = noise.gb*2 - 1;
    vec3 dir = normalize(n*noise.r + r1 * noise.g + r2*noise.b);

    Ray ray;
    ray.origin = pos +0.01*normal;
    ray.dir = dir;
    Intersect inter = intersect(ray);
    if(inter.light){
      ld.ambient += (inter.light_color)/float(u_num_ao_rays);
    }
    else if(inter.depth > RAY_RANGE){
        ld.ambient += (u_ambient_color)/float(u_num_ao_rays);
    }
  }

  return ld;
}

struct Reflect{
  vec3 color;
  float reflection;
  float depth;
  vec3 dir; 
};
uniform float u_ambient_scalar;
uniform float u_diffuse_scalar;

Reflect getReflection(in Ray ray, in vec3 normal, in Material mat){
  ray.dir = reflect(ray.dir,normal);

  vec3 blue = texture(u_blue_noise,gl_FragCoord.xy/u_bn_size).xyz;
  int j = u_frame_num+1;
  vec3 popo = vec3(IRATIONAL_0*(j%97), IRATIONAL_1*(j%137), IRATIONAL_2*(j%67));
  vec3 noise = mod(blue+popo,1.0)*0.5 - 0.5;

  ray.dir = normalize(ray.dir+ noise*mat.roughness);
  ray.origin+=ray.dir*0.001;
  Intersect inter = intersect(ray);
  if(inter.light){
    return Reflect(inter.light_color,0,inter.depth,ray.dir);
  }
  if(inter.depth < RAY_RANGE){
    ray.origin+=ray.dir*inter.depth;
    Material ref_mat = u_materials[inter.mat];
    LightData ld = getLight(ray.origin, inter.normal, ref_mat);
    return Reflect(ref_mat.color*(ld.diffuse*u_diffuse_scalar + ld.ambient*u_ambient_scalar),ref_mat.reflection,inter.depth,ray.dir);
  }
  if(inter.light){
    return Reflect(inter.light_color,0,0,ray.dir);
  }
  return Reflect(vec3(0),0,0,ray.dir);
}

#define NUM_REFLECTION_RAYS 16

void main()
{
  Ray ray;
  ray.origin = u_cam.origin; 

  // get ray directon
  vec4 wp = inverse(u_cam.view_projection)*vec4(v_tex_coords*2-1,0.0,1);
  wp/=wp.w;
  ray.dir = normalize(vec3(wp) - ray.origin);

  // first ray intersection
  // ...

  out_comp = vec4(0,0,0,1);
  Intersect fp = intersect(ray);
  float comp_max = 10;
  if(fp.light){
    out_albedo = vec4(fp.light_color,1);
    out_diffuse = vec4(fp.light_color,1);
    out_ambient = vec4(fp.light_color,1);
    out_comp = vec4(vec2(0,1)+out_comp.rg/comp_max,0,1);
    ray.origin+=ray.dir*fp.depth;
    vec3 pos = ray.origin;
    vec4 proj = u_cam.view_projection*vec4(pos,1);
    gl_FragDepth = proj.z/proj.w;
    return;
  }
  if(fp.depth > RAY_RANGE){
    out_comp = vec4(vec2(0,1)+out_comp.rg/comp_max,0,1);
    gl_FragDepth = 1;
    return;
  }
   
  Material mat = u_materials[fp.mat];
  ray.origin+=ray.dir*fp.depth;
  vec3 pos = ray.origin;

  LightData ld = getLight(ray.origin, fp.normal, mat);


  vec3 reflection = vec3(0);
  float ref = mat.reflection;
  for(int i=0;i<u_reflection_depth;++i){
    if(ref<0.001)break;
    Reflect refl = getReflection(ray,fp.normal,mat); 
    reflection += refl.color*ref*(1-refl.reflection);
    
    ray.dir = refl.dir;
    ray.origin+=ray.dir*refl.depth;
    ref*=refl.reflection;
  }


  vec4 proj = u_cam.view_projection*vec4(pos,1);
  gl_FragDepth = proj.z/proj.w;


  out_albedo = vec4(mat.color,1);
  out_normal = vec4(fp.normal*0.5+0.5,1);
  out_position = vec4(pos,1);
  out_diffuse = vec4(ld.diffuse,1);
  out_ambient = vec4(ld.ambient,1);
  out_reflection = vec4(reflection,mat.reflection);
  out_comp = vec4(vec2(0,1)+out_comp.rg/comp_max,0,1);
}
