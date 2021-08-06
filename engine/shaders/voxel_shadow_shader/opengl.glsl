#type vertex
#version 330 core



layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_tex_coords;


uniform mat4 u_view_projection;
uniform vec3 u_origin;
uniform vec3 u_box_position;
uniform vec3 u_box_size;


out vec3 v_position;
out vec3 v_origin;
out vec3 v_direction;
out vec2 v_tex_coords;



void main()
{
    v_position = a_position;
    v_tex_coords = a_tex_coords;
    gl_Position =  vec4(a_position,1);
}
#type fragment
#version 330 core
layout(location = 0) out vec4 out_color;


uniform vec3 u_texture_size;
uniform vec3 u_box_position;
uniform vec3 u_box_size;
uniform vec3 u_origin;
uniform mat4 u_transform;
uniform mat4 u_view_projection;

uniform sampler3D u_texture;
uniform vec2 u_window_size;
uniform sampler2D u_albedo;
uniform sampler2D u_normal;
uniform sampler2D u_position;
uniform sampler2D u_blue_noise;
uniform int u_show_shadow_caster;
uniform int u_frame_number;

struct Light{
  vec3 position;
  float long_range;
  float short_range;
  vec3 color;
};
uniform int u_num_lights;
uniform Light u_lights[32];

in vec3 v_position;
in vec3 v_origin;
in vec3 v_direction;
in vec2 v_tex_coords;


uint GetVoxel(in vec3 coords){
    return uint(texture(u_texture,coords/u_texture_size)*255);
}

struct Ray{
    vec3 origin;
    vec3 direction;

};
struct RayHit{
    bool hit;
    float distance;
};
RayHit GetOctreeHit(in uint value, in RayHit ray_hit,in Ray ray,in vec3 c,in vec3 b_size,in vec3 t_size, in float max_length){
  ray.origin = ray.origin + ray.direction*ray_hit.distance;

  vec3 voxel_size = (b_size/t_size)/2.0f;
  vec3 octpos = c*b_size/t_size + voxel_size - b_size/2;
  vec3 dir = ray.origin - octpos;
  b_size=b_size/t_size;
  t_size = vec3(2);
  vec3 norm = dir/b_size +0.5;// position of hit 0-1
  vec3 coords=clamp(floor(norm * 2), vec3(0), vec3(1));
  vec3 steps = vec3(sign(ray.direction.x), sign(ray.direction.y), sign(ray.direction.z));

  float outOfX = steps.x < 0? -1:2;
  float outOfY = steps.y < 0? -1:2;
  float outOfZ = steps.z < 0? -1:2;

  vec3 plane_steps = vec3(steps.x<0?0:1,steps.y<0?0:1,steps.z<0?0:1);
  vec3 planes = vec3((((coords + plane_steps)/t_size) - 0.5) * b_size);

  float distX = abs(dir.x - planes.x);
  float distY = abs(dir.y - planes.y);
  float distZ = abs(dir.z - planes.z);
  float tMaxX = abs(distX/ray.direction.x);
  float tMaxY = abs(distY/ray.direction.y);
  float tMaxZ = abs(distZ/ray.direction.z);
    
  vec3 tDelta = abs(voxel_size/ray.direction);
  vec4 color = vec4(0,0,0,1);
    while(true){
        if(ray_hit.distance > max_length){
            break;
        }
        uint v = value&(uint(1)<<int(coords.z*4+coords.y*2 + coords.x));
        if( v != uint(0)){
            ray_hit.hit = true;
            break;

        }
        if(tMaxX < tMaxY) {
            if(tMaxX < tMaxZ) {
                coords.x += steps.x;
                ray_hit.distance = tMaxX;
                if(abs(coords.x - outOfX)<0.2) break;
                tMaxX= tMaxX + tDelta.x;
            } else {
                coords.z += steps.z;
                ray_hit.distance = tMaxZ;
                if(abs(coords.z - outOfZ)<0.2) break;
                tMaxZ= tMaxZ + tDelta.z;
            }
        } else {
            if(tMaxY < tMaxZ) {
                coords.y += steps.y;
                ray_hit.distance = tMaxY;
                if(abs(coords.y - outOfY)<0.2) break;
                tMaxY= tMaxY + tDelta.y;
            } else {
                coords.z += steps.z;
                ray_hit.distance = tMaxZ;
                if(abs(coords.z - outOfZ)<0.2) break;
                tMaxZ= tMaxZ + tDelta.z;
            }
        }
    }
  return ray_hit;
}

RayHit algo(in Ray ray,  in vec3 b_size, in vec3 t_size, in float max_length){
    
    // MOVE RAY TO BOX
    vec3 sides_dist = -(sign(ray.direction) / 2) * b_size ;
    sides_dist -=  ray.origin;
    vec3 outside = clamp(sign(abs(ray.origin) - b_size / 2), 0,1);// 1 if outside, 0 if not
    vec3 distances = abs(sides_dist / ray.direction) * outside;
    float d_to_box = max(distances.x, max(distances.y, distances.z));
    ray.origin += ray.direction * d_to_box;

    RayHit ray_hit;
    ray_hit.hit = false;
    ray_hit.distance = d_to_box;

    vec3 voxel_size = b_size/t_size;
    vec3 dir = ray.origin;
    vec3 norm = ray.origin/b_size + 0.5;


    vec3 coords = clamp(floor(norm * t_size), vec3(0), t_size - vec3(1));

    vec3 steps = vec3(sign(ray.direction.x),sign(ray.direction.y),sign(ray.direction.z));


    float outOfX = steps.x < 0? -1:t_size.x;
    float outOfY = steps.y < 0? -1:t_size.y;
    float outOfZ = steps.z < 0? -1:t_size.z;

    vec3 plane_steps = vec3(steps.x<0?0:1,steps.y<0?0:1,steps.z<0?0:1);
    vec3 planes = vec3((((coords + plane_steps)/t_size) - 0.5) * b_size);


    float distX = abs(dir.x - planes.x);
    float distY = abs(dir.y - planes.y);
    float distZ = abs(dir.z - planes.z);
    float tMaxX = abs(distX/ray.direction.x);
    float tMaxY = abs(distY/ray.direction.y);
    float tMaxZ = abs(distZ/ray.direction.z);





    vec3 tDelta = vec3(abs(voxel_size.x/ray.direction.x), abs(voxel_size.y/ray.direction.y), abs(voxel_size.z/ray.direction.z));




    while(true){
        if(ray_hit.distance > max_length){
            break;
        }
        uint voxel = GetVoxel(coords);
        if( voxel != uint(0)){
            RayHit hit = GetOctreeHit(voxel,ray_hit, ray,coords, b_size, t_size, max_length);
            if(hit.hit)
              return hit;
        }
        if(tMaxX < tMaxY) {
            if(tMaxX < tMaxZ) {
                coords.x += steps.x;
                ray_hit.distance = tMaxX;
                if(abs(coords.x - outOfX)<0.2) break;
                tMaxX= tMaxX + tDelta.x;
            } else {
                coords.z += steps.z;
                ray_hit.distance = tMaxZ;
                if(abs(coords.z - outOfZ)<0.2) break;
                tMaxZ= tMaxZ + tDelta.z;
            }
        } else {
            if(tMaxY < tMaxZ) {
                coords.y += steps.y;
                ray_hit.distance = tMaxY;
                if(abs(coords.y - outOfY)<0.2) break;
                tMaxY= tMaxY + tDelta.y;
            } else {
                coords.z += steps.z;
                ray_hit.distance = tMaxZ;
                if(abs(coords.z - outOfZ)<0.2) break;
                tMaxZ= tMaxZ + tDelta.z;
            }
        }
    }

    return ray_hit;

}


void main()
{
    vec3 position = texture(u_position,v_tex_coords).xyz;
    vec3 normal = texture(u_normal,v_tex_coords).xyz;
    vec3 albedo = texture(u_albedo,v_tex_coords).xyz;
    vec3 blue = texture(u_blue_noise,v_tex_coords).xyz;
    out_color = vec4(blue,1);
    return;

    Ray ray;
    ray.origin = u_origin;
    ray.direction = normalize(position - u_origin);

    /* vec3 position = u_origin + dist * ray.direction; */ 
    //ambient occlusion

    if(u_show_shadow_caster == 1){
      RayHit hit = algo(ray,u_box_size,u_texture_size, 100);
      if(hit.hit){
        out_color = vec4(0.25,0.75, 0.25,1);
        return;
      }
    }



    //shadows
    vec3 light = vec3(0);
    for(int i=0;i<u_num_lights;++i){
        ray.origin = position+0.1*normal;
        vec3 dir = u_lights[i].position - ray.origin;
        if(length(dir)>u_lights[i].long_range){
          continue;
        }
        ray.direction = normalize(dir);
        float diff = max(dot(ray.direction, normal),0.0);
        if(diff<0.01){
          continue;
        }
        RayHit hit = algo(ray,u_box_size,u_texture_size, length(dir));
        if(hit.hit==false){
          light = u_lights[i].color;
        }
    }
    out_color = vec4(albedo*(0.25 + 0.75*light),1);//*color;
}
