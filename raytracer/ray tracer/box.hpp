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
    box(const Vector3f& p0, const Vector3f& p1, material *ptr);
    virtual bool hit(const Ray& r, float t0, float t1, SurfaceInteraction& rec) const;
    virtual bool bounding_box(float t0, float t1, aabb& box) const {
        box =  aabb(pmin, pmax);
        return true; }
    
    
    Vector3f pmin, pmax;
    hitable *list_ptr;
};

#endif /* box_hpp */
