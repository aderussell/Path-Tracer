//
//  hittable_sets.hpp
//  raytracer
//
//  Created by Adrian Russell on 24/05/2020.
//  Copyright © 2020 Adrian Russell. All rights reserved.
//

#ifndef hittable_sets_hpp
#define hittable_sets_hpp

#include "hitable.hpp"

class hitable_union: public hitable {
public:
    //hitable_union() {}
    hitable_union(hitable *left, hitable *right): left(left), right(right) {}
    
    virtual bool hit(const Ray&r, float t_min, float t_max, SurfaceInteraction& rec) const;
    virtual bool bounding_box(float t0, float t1, aabb &box) const;
    virtual float pdf_value(const Vector3f& o, const Vector3f& v) const;
    virtual Vector3f random(const Vector3f& o) const;
    
    hitable *left, *right;
};

class hitable_difference: public hitable {
public:
    //hitable_difference() {}
    hitable_difference(hitable *left, hitable *right): left(left), right(right) {}
    
    virtual bool hit(const Ray&r, float t_min, float t_max, SurfaceInteraction& rec) const;
    virtual bool bounding_box(float t0, float t1, aabb &box) const;
    virtual float pdf_value(const Vector3f& o, const Vector3f& v) const;
    virtual Vector3f random(const Vector3f& o) const;
    
    hitable *left, *right;
};

class hitable_intersection: public hitable {
public:
    //hitable_intersection() {}
    hitable_intersection(hitable *left, hitable *right): left(left), right(right) {}
    
    virtual bool hit(const Ray&r, float t_min, float t_max, SurfaceInteraction& rec) const;
    virtual bool bounding_box(float t0, float t1, aabb &box) const;
    virtual float pdf_value(const Vector3f& o, const Vector3f& v) const;
    virtual Vector3f random(const Vector3f& o) const;
    
    hitable *left, *right;
};

#endif /* hittable_sets_hpp */
