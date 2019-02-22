//
//  isotropic.cpp
//  raytracer
//
//  Created by Adrian Russell on 16/12/2018.
//  Copyright © 2018 Adrian Russell. All rights reserved.
//

#include "isotropic.hpp"
#include <stdlib.h>

Vector3f isotropic::random_in_unit_sphere() const {
    Vector3f p;
    do {
        p = 2.0 * Vector3f(drand48(), drand48(), drand48()) - Vector3f(1,1,1);
    } while (p.squareMagnitude() >= 1.0);
    return p;
}

bool isotropic::scatter(const Ray& r_in, const hit_record& rec, scatter_record& srec) const  {
    srec.specular_ray = Ray(rec.p, this->random_in_unit_sphere(), r_in.time());
    srec.attenuation = albedo->value(rec.u, rec.v, rec.p);
    return true;
}
