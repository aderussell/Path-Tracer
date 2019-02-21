//
//  sphere.hpp
//  raytracer
//
//  Created by Adrian Russell on 06/12/2018.
//  Copyright © 2018 Adrian Russell. All rights reserved.
//

#ifndef sphere_hpp
#define sphere_hpp

#include "hitable.hpp"

class sphere: public hitable {

public:
    sphere() {}
    sphere(Vector3f center, float radius, material* m) : center(center), radius(radius), mat(m) {}
    Vector3f center;
    float radius;
    material* mat;
    
    bool hit(const ray&r, float t_min, float t_max, hit_record& rec) const;
    bool bounding_box(float t0, float t1, aabb &box) const;
    virtual float pdf_value(const Vector3f& o, const Vector3f& v) const;
    virtual Vector3f random(const Vector3f& o) const;
};


class moving_sphere: public hitable {
    
public:
    moving_sphere() {}
    moving_sphere(Vector3f cen0, Vector3f cen1, float t0, float t1, float radius, material* m) : center0(cen0), center1(cen1), time0(t0), time1(t1), radius(radius), mat(m) {}
    Vector3f center0, center1;
    float time0, time1;
    float radius;
    material* mat;
    
    Vector3f center(float time) const;
    
    bool hit(const ray&r, float t_min, float t_max, hit_record& rec) const;
    bool bounding_box(float t0, float t1, aabb &box) const;
};


#endif /* sphere_hpp */
