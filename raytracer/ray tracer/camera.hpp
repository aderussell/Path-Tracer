//
//  camera.hpp
//  raytracer
//
//  Created by Adrian Russell on 06/12/2018.
//  Copyright © 2018 Adrian Russell. All rights reserved.
//

#ifndef camera_hpp
#define camera_hpp

#include "Ray.hpp"
#include <stdlib.h>

class camera {
public:
    virtual ray get_ray(float u, float v) = 0;
};

class cameraA: public camera {
public:
    cameraA(float vfov, float aspect) {
        float theta = vfov * M_PI/180.0;
        float half_height = tan(theta/2);
        float half_width = aspect * half_height;
        lowerLeftCorner = Vector3(-half_width, -half_height, -1.0);
        horizontal= Vector3(2*half_width, 0.0, 0.0);
        vertical= Vector3(0.0, 2*half_height, 0.0);
        origin= Vector3(0.0, 0.0, 0.0);
    }
    Vector3 lowerLeftCorner;
    Vector3 horizontal;
    Vector3 vertical;
    Vector3 origin;
    
    ray get_ray(float u, float v) { return ray(origin, lowerLeftCorner + (u * horizontal) + (v * vertical)); }
};

class cameraB: public camera {
public:
    cameraB(Vector3 lookfrom, Vector3 lookat, Vector3 vup, float vfov, float aspect) {
        Vector3 u, v, w;
        float theta = vfov * M_PI/180.0;
        float half_height = tan(theta/2);
        float half_width = aspect * half_height;
        origin = lookfrom;
        w = (lookfrom - lookat).normalise();
        u = Vector3::crossProduct(vup, w).normalise();
        v = Vector3::crossProduct(w, u);
        //lowerLeftCorner = Vector3(-half_width, -half_height, -1.0);
        lowerLeftCorner = origin - half_width*u - half_height*v - w;
        horizontal= u * 2*half_width;
        vertical= v * 2*half_height;
    }
    Vector3 lowerLeftCorner;
    Vector3 horizontal;
    Vector3 vertical;
    Vector3 origin;
    
    ray get_ray(float u, float v) { return ray(origin, lowerLeftCorner + (u * horizontal) + (v * vertical) - origin); }
};


class cameraC: public camera {
public:
    cameraC(Vector3 lookfrom, Vector3 lookat, Vector3 vup, float vfov, float aspect, float aperture, float focus_dist) {
        lens_radius = aperture / 2.0;
        float theta = vfov * M_PI/180.0;
        float half_height = tan(theta/2);
        float half_width = aspect * half_height;
        origin = lookfrom;
        w = (lookfrom - lookat).normalise();
        u = Vector3::crossProduct(vup, w).normalise();
        v = Vector3::crossProduct(w, u);
        //lowerLeftCorner = Vector3(-half_width, -half_height, -1.0);
        lowerLeftCorner = origin - half_width*focus_dist*u - half_height*focus_dist*v - w*focus_dist;
        horizontal= u * 2*half_width*focus_dist;
        vertical= v * 2*half_height*focus_dist;
    }
    Vector3 lowerLeftCorner;
    Vector3 horizontal;
    Vector3 vertical;
    Vector3 origin;
    Vector3 u, v, w;
    float lens_radius;
    
    Vector3 random_in_unit_disk() {
        Vector3 p;
        do {
            p = 2.0 * Vector3(drand48(), drand48(), 0) - Vector3(1,1,0);
        } while (Vector3::dotProduct(p, p) >= 1.0);
        return p;
    }
    
    ray get_ray(float u, float v) {
        Vector3 rd = lens_radius * this->random_in_unit_disk();
        Vector3 offset = u * rd.x + v * rd.y;
        return ray(origin + offset, lowerLeftCorner + (u * horizontal) + (v * vertical) - origin - offset);
        
    }
};

#endif /* camera_hpp */
