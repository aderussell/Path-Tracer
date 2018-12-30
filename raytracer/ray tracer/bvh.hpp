//
//  bvh.hpp
//  raytracer
//
//  Created by Adrian Russell on 09/12/2018.
//  Copyright © 2018 Adrian Russell. All rights reserved.
//

#ifndef bvh_hpp
#define bvh_hpp

#include "hitable.hpp"

class bvh_node : public hitable {
public:
    bvh_node() {}
    bvh_node(hitable **l, int n, float time0 = 0.0, float time1 = 1.0);
    
    bool hit(const ray&r, float t_min, float t_max, hit_record& rec) const;
    bool bounding_box(float t0, float t1, aabb &box) const;
    
    float pdf_value(const Vector3 &o, const Vector3 &v) const;
    Vector3 random(const Vector3 &o) const;
    
private:
    aabb box;
    hitable *left;
    hitable *right;
};

#endif /* bvh_hpp */
