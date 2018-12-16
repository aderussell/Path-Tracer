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
    translate(hitable *p, const Vector3& displacement) : ptr(p), offset(displacement) {}
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
    virtual bool bounding_box(float t0, float t1, aabb& box) const;
    hitable *ptr;
    Vector3 offset;
};

class rotate_y : public hitable {
public:
    rotate_y(hitable *p, float angle);
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
    virtual bool bounding_box(float t0, float t1, aabb& box) const {
        box = bbox; return hasbox;}
    hitable *ptr;
    float sin_theta;
    float cos_theta;
    bool hasbox;
    aabb bbox;
};


#endif /* translate_hpp */
