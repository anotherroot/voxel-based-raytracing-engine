#type vertex
#version 330 core




layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_tex_coords;


uniform mat4 u_view_projection;
uniform mat4 u_transform;
uniform vec3 u_origin;
uniform vec3 u_box_position;
uniform vec3 u_box_size;


out vec3 v_position;
out vec3 v_origin;
out vec3 v_direction;
out vec3 v_tex_coords;




void main()
{
    vec3 pos = vec3(u_transform* vec4(a_position,1));
    v_position = pos;

    // we do this to get the relation between the box and the camera without rotation
    vec3 camera_local = vec3(inverse(u_transform)*vec4(u_origin,1));
    vec3 dir = pos - u_origin;
    dir = vec3(inverse(u_transform)*vec4(dir,0));

    v_direction = dir*u_box_size;
    v_origin = camera_local*u_box_size;

    v_tex_coords = a_tex_coords;
    gl_Position =  u_view_projection*vec4(pos,1);
}
#type fragment
#version 330 core

layout(location = 0) out vec4 out_color;

layout(location = 1) out vec4 out_normal;
layout(location = 2) out vec4 out_position;
layout(location = 3) out vec4 out_buffer;

uniform vec3 u_texture_size;
uniform vec3 u_box_position;
uniform vec3 u_box_size;
uniform vec3 u_origin;
uniform mat4 u_transform;
uniform mat4 u_view_projection;

uniform sampler3D u_texture;

struct Material{
    vec3 color;
};
uniform Material u_materials[256];


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
in vec3 v_tex_coords;


uint GetVoxel(in vec3 coords){
    return uint(texture(u_texture,coords/u_texture_size)*255)  ;
}

struct Ray{
    vec3 origin;
    vec3 direction;
};

struct RayHit{
    bool hit;
    vec3 normal;
    float distance;
    int material;
};


RayHit algo(in Ray ray,  in vec3 b_pos, in vec3 b_size, in vec3 t_size, in vec3 in_normal, in float len){
    RayHit ray_hit;
    ray_hit.hit = false;
    ray_hit.distance = 0;
    ray_hit.normal = vec3(0,0,0);





    vec3 voxel_size = b_size/t_size;

    vec3 dir = ray.origin;

    vec3 norm = ray.origin/b_size + 0.5;

    vec3 f_norm = floor(norm);
    vec3 c_norm = ceil(norm);







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


    ray_hit.normal = in_normal;


    //return coords.z/32.;
    //vec3 tMax = vec3(tMaxX, tMaxY, tMaxZ);
    //return vec3(outOfX/2.+0.5, outOfY/2.+0.5, 0);

    vec3 tDelta = vec3(abs(voxel_size.x/ray.direction.x), abs(voxel_size.y/ray.direction.y), abs(voxel_size.z/ray.direction.z));




    while(true){
        uint voxel = GetVoxel(coords);
        if(ray_hit.distance > len) break;
        if( voxel != uint(0)){
            ray_hit.material = int(voxel)-1;
            ray_hit.hit = true;

            return ray_hit;
        }
        if(tMaxX < tMaxY) {
            if(tMaxX < tMaxZ) {
                coords.x += steps.x;
                ray_hit.normal = vec3(-steps.x,0,0);
                ray_hit.distance = tMaxX;
                if(abs(coords.x - outOfX)<0.2) break;


                tMaxX= tMaxX + tDelta.x;

            } else {
                coords.z += steps.z;
                ray_hit.normal = vec3(0,0,-steps.z);
                ray_hit.distance = tMaxZ;
                if(abs(coords.z - outOfZ)<0.2) break;

                tMaxZ= tMaxZ + tDelta.z;
            }
        } else {
            if(tMaxY < tMaxZ) {
                coords.y += steps.y;
                ray_hit.normal = vec3(0,-steps.y,0);
                ray_hit.distance = tMaxY;
                if(abs(coords.y - outOfY)<0.2) break;

                tMaxY= tMaxY + tDelta.y;
            } else {
                coords.z += steps.z;
                ray_hit.normal = vec3(0,0,-steps.z);
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
    Ray ray;
    ray.origin = v_origin;
    ray.direction = normalize(v_direction);

    // MOVE RAY TO BOX
    vec3 sides_dist = -(sign(ray.direction) / 2) * u_box_size ;
    sides_dist -=  ray.origin;

    vec3 outside = clamp(sign(abs(ray.origin) - u_box_size / 2), 0,1);// 1 if outside, 0 if not
    vec3 distances = abs(sides_dist / ray.direction) * outside;
    float d_to_box = max(distances.x, max(distances.y, distances.z));
    ray.origin += ray.direction * d_to_box;

    vec3 d_sign = sign(ray.origin);
    vec3 in_normal = vec3(0,0,d_sign.z);
    if(distances.x > distances.y){
        if(distances.x > distances.z){
            in_normal = vec3(d_sign.x,0,0);
        }
    }
    else {
        if(distances.y > distances.z){
            in_normal = vec3(0,d_sign.y,0);
        }
    }



    RayHit hit = algo(ray, u_box_position, u_box_size, u_texture_size, in_normal,100);
    if(hit.hit){
        vec4 normal = vec4(normalize(vec3(u_transform*vec4(hit.normal,0))/u_box_size),1);
        out_normal = normal/2 +0.5;

        vec3 dir = normalize(v_position - u_origin);
        vec3 pos = u_origin + (d_to_box + hit.distance)*dir;
        vec4 proj = u_view_projection*vec4(pos,1);
        vec3 s = v_position;
        out_position = vec4(pos,1);//if you dont put vec4 here it gives you really weird results
        gl_FragDepth = proj.z/proj.w;

        /* /1* float diffuse = 0.75 + 0.25 * clamp(dot(normalize(vec3(2,3,1)),normal.xyz),-1,1); *1/ */
        /* //diffuse = 1; */
        /* /1* vec4 color = vec4(u_materials[hit.material].color*diffuse,1); *1/ */
        /* uint shadow = uint(0); */
        /* if(int(u_texture_size.x) != 1){ */
        /* for(int i=0;i<u_num_lights;++i){ */
        /*   vec3 p = vec3(inverse(u_transform)*vec4(u_lights[i].position,1)); */
        /*   p*= u_box_size; */ 
        /*   ray.origin = ray.origin + hit.distance*ray.direction + hit.normal.xyz*0.001; */
        /*   vec3 dir = p - ray.origin; */ 
        /*   ray.direction =normalize(dir); */ 
        /*   if(length(dir)>u_lights[i].long_range){ */
        /*     shadow|=uint(1)<<i; */
        /*     continue; */
        /*   } */

        /*   float diff = max(dot(ray.direction, hit.normal),0.0); */
        /*   if(diff<0.001){ */
        /*     shadow|=uint(1)<<i; */
        /*     continue; */
        /*   } */

        /*   RayHit shadow_hit = algo(ray,u_box_position, u_box_size, u_texture_size, in_normal, length(dir)); */   
        /*   if(shadow_hit.hit){ */
        /*     shadow|=uint(1)<<i; */
        /*   } */
        /* } */
        /* } */
        /* out_buffer = vec4(float(shadow)/256,0,0,1); */

        vec4 color = vec4(u_materials[hit.material].color,1);
        out_color=color;

    }
    else{
        gl_FragDepth=1;
        discard;
    }


}
