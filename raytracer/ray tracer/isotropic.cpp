//
//  isotropic.cpp
//  raytracer
//
//  Created by Adrian Russell on 16/12/2018.
//  Copyright © 2018 Adrian Russell. All rights reserved.
//

#include "isotropic.hpp"
#include <stdlib.h>

Vector3 isotropic::random_in_unit_sphere() const {
    Vector3 p;
    do {
        p = 2.0 * Vector3(drand48(), drand48(), drand48()) - Vector3(1,1,1);
    } while (p.squareMagnitude() >= 1.0);
    return p;
}

bool isotropic::scatter(const ray& r_in, const hit_record& rec, Vector3& attenuation, ray& scattered) const  {
    scattered = ray(rec.p, this->random_in_unit_sphere());
    Color color = albedo->value(rec.u, rec.v, rec.p);
    attenuation = Vector3(color.r, color.g, color.b);
    return true;
}
