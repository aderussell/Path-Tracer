//
//  box.hpp
//  raytracer
//
//  Created by Adrian Russell on 16/12/2018.
//  Copyright © 2018 Adrian Russell. All rights reserved.
//

#ifndef box_hpp
#define box_hpp

#include "hitable_list.hpp"
#include "xy_rect.hpp"

class box: public hitable  {
public:
    box() {}
    box(const Vector3& p0, const Vector3& p1, material *ptr);
    virtual bool hit(const ray& r, float t0, float t1, hit_record& rec) const;
    virtual bool bounding_box(float t0, float t1, aabb& box) const {
        box =  aabb(pmin, pmax);
        return true; }
    
    
    Vector3 pmin, pmax;
    hitable *list_ptr;
};

#endif /* box_hpp */
