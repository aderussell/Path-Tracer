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
};

#endif /* sphere_hpp */
