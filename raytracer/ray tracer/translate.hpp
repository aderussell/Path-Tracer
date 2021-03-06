//
//  translate.hpp
//  raytracer
//
//  Created by Adrian Russell on 16/12/2018.
//  Copyright © 2018 Adrian Russell. All rights reserved.
//

#ifndef translate_hpp
#define translate_hpp

#include "hitable.hpp"
#include <cfloat>

class translate : public hitable {
public:
    translate(hitable *p, const Vector3f& displacement) : ptr(p), offset(displacement) {}
    virtual bool hit(const Ray& r, float t_min, float t_max, SurfaceInteraction& rec) const;
    virtual bool bounding_box(float t0, float t1, aabb& box) const;
    
    virtual Vector3f random(const Vector3f& o) const { return Vector3f(1,0,0); }
    
    hitable *ptr;
    Vector3f offset;
};

class rotate_y : public hitable {
public:
    rotate_y(hitable *p, float angle);
    virtual bool hit(const Ray& r, float t_min, float t_max, SurfaceInteraction& rec) const;
    virtual bool bounding_box(float t0, float t1, aabb& box) const {
        box = bbox; return hasbox;}
    
    virtual Vector3f random(const Vector3f& o) const { return Vector3f(1,0,0); }
    
    hitable *ptr;
    float sin_theta;
    float cos_theta;
    bool hasbox;
    aabb bbox;
};

class rotate_x : public hitable {
public:
    rotate_x(hitable *p, float angle);
    virtual bool hit(const Ray& r, float t_min, float t_max, SurfaceInteraction& rec) const;
    virtual bool bounding_box(float t0, float t1, aabb& box) const {
        box = bbox; return hasbox;}
    
    virtual Vector3f random(const Vector3f& o) const { return Vector3f(1,0,0); }
    
    hitable *ptr;
    float sin_theta;
    float cos_theta;
    bool hasbox;
    aabb bbox;
};

class rotate_z : public hitable {
public:
    rotate_z(hitable *p, float angle);
    virtual bool hit(const Ray& r, float t_min, float t_max, SurfaceInteraction& rec) const;
    virtual bool bounding_box(float t0, float t1, aabb& box) const {
        box = bbox; return hasbox;}
    
    virtual Vector3f random(const Vector3f& o) const { return Vector3f(1,0,0); }
    
    hitable *ptr;
    float sin_theta;
    float cos_theta;
    bool hasbox;
    aabb bbox;
};

class scale : public hitable {
public:
    scale(hitable *p, float _s) : ptr(p), s(_s) {}
    virtual bool hit(const Ray& r, float t_min, float t_max, SurfaceInteraction& rec) const;
    virtual bool bounding_box(float t0, float t1, aabb& box) const;
    
    virtual Vector3f random(const Vector3f& o) const { return Vector3f(1,0,0); }
    
    hitable *ptr;
    float s;
};

class scale2 : public hitable {
public:
    scale2(hitable *p, Vector3f _s) : ptr(p), s(_s) {}
    virtual bool hit(const Ray& r, float t_min, float t_max, SurfaceInteraction& rec) const;
    virtual bool bounding_box(float t0, float t1, aabb& box) const;
    
    virtual Vector3f random(const Vector3f& o) const { return Vector3f(1,0,0); }
    
    hitable *ptr;
    Vector3f s;
};


#endif /* translate_hpp */
