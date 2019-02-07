//
//  metal.cpp
//  raytracer
//
//  Created by Adrian Russell on 07/12/2018.
//  Copyright © 2018 Adrian Russell. All rights reserved.
//

#include "metal.hpp"
#include <stdlib.h>

Vector3 metal::reflect(const Vector3& v, const Vector3& n) const {
    return v - 2 * Vector3::dotProduct(v, n) * n;
}

Vector3 metal::random_in_unit_sphere() const {
    Vector3 p;
    do {
        p = 2.0 * Vector3(drand48(), drand48(), drand48()) - Vector3(1,1,1);
    } while (p.squareMagnitude() >= 1.0);
    return p;
}

bool metal::scatter(const ray& ray_in, const hit_record& hrec, scatter_record& srec) const {
    Vector3 reflected = reflect(ray_in.direction().normalise(), hrec.normal);
    srec.specular_ray = ray(hrec.p, reflected + fuzz * this->random_in_unit_sphere(), ray_in.time());
    srec.attenuation = albedo;
    srec.is_specular = true;
    srec.pdf_ptr = nullptr;
    return true;
}
