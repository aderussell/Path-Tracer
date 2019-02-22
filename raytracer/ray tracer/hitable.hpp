//
//  hitable.hpp
//  raytracer
//
//  Created by Adrian Russell on 06/12/2018.
//  Copyright © 2018 Adrian Russell. All rights reserved.
//

#ifndef hitable_hpp
#define hitable_hpp

#include "Vector3.hpp"
#include "ray.hpp"
#include "aabb.hpp"

class material;

struct hit_record {
    float t;            // time
    Vector3f p;          // position
    Vector3f normal;
    material *mat_ptr;
    float u, v;
};

class hitable {
public:
    virtual bool hit(const Ray&r, float t_min, float t_max, hit_record& rec) const = 0;
    virtual bool bounding_box(float t0, float t1, aabb& box) const = 0;
    
    virtual float pdf_value(const Vector3f& o, const Vector3f& v) const { return 0.0; }
    virtual Vector3f random(const Vector3f& o) const { return Vector3f(1,0,0); }
};

class flip_normals : public hitable {
public:
    flip_normals(hitable *p) : ptr(p) {}
    virtual bool hit(const Ray& r, float t_min, float t_max, hit_record& rec) const {
        if (ptr->hit(r, t_min, t_max, rec)) {
            rec.normal *= -1;
            return true;
        }
        else
            return false;
    }
    virtual bool bounding_box(float t0, float t1, aabb& box) const {
        return ptr->bounding_box(t0, t1, box);
    }
    hitable *ptr;
};


#endif /* hitable_hpp */
