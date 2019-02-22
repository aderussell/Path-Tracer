//
//  hitable_list.hpp
//  raytracer
//
//  Created by Adrian Russell on 06/12/2018.
//  Copyright © 2018 Adrian Russell. All rights reserved.
//

#ifndef hitable_list_hpp
#define hitable_list_hpp

#include "hitable.hpp"

class hitable_list: public hitable {
public:
    hitable_list() {}
    hitable_list(hitable **l, int n) { list = l; list_size = n; }
    hitable **list;
    int list_size;
    
    bool hit(const Ray&r, float t_min, float t_max, hit_record& rec) const;
    bool bounding_box(float t0, float t1, aabb &box) const;
    virtual float pdf_value(const Vector3f& o, const Vector3f& v) const;
    virtual Vector3f random(const Vector3f& o) const;
};

#endif /* hitable_list_hpp */
