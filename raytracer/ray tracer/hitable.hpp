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
#include "Ray.hpp"

class material;

struct hit_record {
    float t;
    Vector3 p;
    Vector3 normal;
    material *mat_ptr;
};

class hitable {
public:
    virtual bool hit(const ray&r, float t_min, float t_max, hit_record& rec) const = 0;
};


#endif /* hitable_hpp */
