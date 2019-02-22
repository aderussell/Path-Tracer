//
//  camera.hpp
//  raytracer
//
//  Created by Adrian Russell on 06/12/2018.
//  Copyright © 2018 Adrian Russell. All rights reserved.
//

#ifndef camera_hpp
#define camera_hpp

#include "ray.hpp"
#include <stdlib.h>

class Camera {
public:
    //virtual ~Camera();
    
    virtual Ray get_ray(float u, float v) = 0;
};

class cameraA: public Camera {
public:
    cameraA(float vfov, float aspect) {
        float theta = vfov * M_PI/180.0;
        float half_height = tan(theta/2);
        float half_width = aspect * half_height;
        lowerLeftCorner = Vector3f(-half_width, -half_height, -1.0);
        horizontal= Vector3f(2*half_width, 0.0, 0.0);
        vertical= Vector3f(0.0, 2*half_height, 0.0);
        origin= Vector3f(0.0, 0.0, 0.0);
    }
    Vector3f lowerLeftCorner;
    Vector3f horizontal;
    Vector3f vertical;
    Vector3f origin;
    
    Ray get_ray(float u, float v) { return Ray(origin, lowerLeftCorner + (u * horizontal) + (v * vertical)); }
};

class cameraB: public Camera {
public:
    cameraB(Vector3f lookfrom, Vector3f lookat, Vector3f vup, float vfov, float aspect) {
        Vector3f u, v, w;
        float theta = vfov * M_PI/180.0;
        float half_height = tan(theta/2);
        float half_width = aspect * half_height;
        origin = lookfrom;
        w = (lookfrom - lookat).normalized();
        u = Vector3f::crossProduct(vup, w).normalized();
        v = Vector3f::crossProduct(w, u);
        //lowerLeftCorner = Vector3f(-half_width, -half_height, -1.0);
        lowerLeftCorner = origin - half_width*u - half_height*v - w;
        horizontal= u * 2*half_width;
        vertical= v * 2*half_height;
    }
    Vector3f lowerLeftCorner;
    Vector3f horizontal;
    Vector3f vertical;
    Vector3f origin;
    
    Ray get_ray(float u, float v) { return Ray(origin, lowerLeftCorner + (u * horizontal) + (v * vertical) - origin); }
};


class cameraC: public Camera {
public:
    cameraC(Vector3f lookfrom, Vector3f lookat, Vector3f vup, float vfov, float aspect, float aperture, float focus_dist, float t0, float t1) {
        time0 = t0;
        time1 = t1;
        lens_radius = aperture / 2.0;
        float theta = vfov * M_PI/180.0;
        float half_height = tan(theta/2);
        float half_width = aspect * half_height;
        origin = lookfrom;
        w = (lookfrom - lookat).normalized();
        u = Vector3f::crossProduct(vup, w).normalized();
        v = Vector3f::crossProduct(w, u);
        //lowerLeftCorner = Vector3f(-half_width, -half_height, -1.0);
        lowerLeftCorner = origin - half_width*focus_dist*u - half_height*focus_dist*v - w*focus_dist;
        horizontal= u * 2*half_width*focus_dist;
        vertical= v * 2*half_height*focus_dist;
    }
    Vector3f lowerLeftCorner;
    Vector3f horizontal;
    Vector3f vertical;
    Vector3f origin;
    Vector3f u, v, w;
    float time0, time1;
    float lens_radius;
    
    Vector3f random_in_unit_disk() {
        Vector3f p;
        do {
            p = 2.0 * Vector3f(drand48(), drand48(), 0) - Vector3f(1,1,0);
        } while (Vector3f::dotProduct(p, p) >= 1.0);
        return p;
    }
    
    Ray get_ray(float s, float t) {
        Vector3f rd = lens_radius * this->random_in_unit_disk();
        Vector3f offset = u * rd.x + v * rd.y;
        float time = time0 + drand48()*(time1-time0);
        return Ray(origin + offset, lowerLeftCorner + (s * horizontal) + (t * vertical) - origin - offset, time);
        
    }
};

#endif /* camera_hpp */
