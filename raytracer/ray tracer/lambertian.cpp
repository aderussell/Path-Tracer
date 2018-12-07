//
//  lambertian.cpp
//  raytracer
//
//  Created by Adrian Russell on 07/12/2018.
//  Copyright © 2018 Adrian Russell. All rights reserved.
//

#include "lambertian.hpp"
#include <stdlib.h>

Vector3 random_in_unit_sphere() {
    Vector3 p;
    do {
        p = 2.0 * Vector3(drand48(), drand48(), drand48()) - Vector3(1,1,1);
    } while (p.squareMagnitude() >= 1.0);
    return p;
}

bool lambertian::scatter(const ray& ray_in, const hit_record& rec, Vector3& attenuation, ray& scattered) const {
    Vector3 target = rec.p + rec.normal + random_in_unit_sphere();
    scattered = ray(rec.p, target - rec.p);
    attenuation = albedo;
    return true;
}
