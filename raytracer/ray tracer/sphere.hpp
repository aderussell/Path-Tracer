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
    sphere(Vector3 center, float radius, material* m) : center(center), radius(radius), mat(m) {}
    Vector3 center;
    float radius;
    material* mat;
    
    bool hit(const ray&r, float t_min, float t_max, hit_record& rec) const;
    bool bounding_box(float t0, float t1, aabb &box) const;
};


class moving_sphere: public hitable {
    
public:
    moving_sphere() {}
    moving_sphere(Vector3 cen0, Vector3 cen1, float t0, float t1, float radius, material* m) : center0(cen0), center1(cen1), time0(t0), time1(t1), radius(radius), mat(m) {}
    Vector3 center0, center1;
    float time0, time1;
    float radius;
    material* mat;
    
    Vector3 center(float time) const;
    
    bool hit(const ray&r, float t_min, float t_max, hit_record& rec) const;
    bool bounding_box(float t0, float t1, aabb &box) const;
};


#endif /* sphere_hpp */
