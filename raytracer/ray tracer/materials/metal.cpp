//
//  metal.cpp
//  raytracer
//
//  Created by Adrian Russell on 07/12/2018.
//  Copyright © 2018 Adrian Russell. All rights reserved.
//

#include "metal.hpp"
#include <stdlib.h>

Vector3f metal::reflect(const Vector3f& v, const Vector3f& n) const {
    return v - 2 * Vector3f::dotProduct(v, n) * n;
}

Vector3f metal::random_in_unit_sphere() const {
    Vector3f p;
    do {
        p = 2.0 * Vector3f(drand48(), drand48(), drand48()) - Vector3f(1,1,1);
    } while (p.squareMagnitude() >= 1.0);
    return p;
}

bool metal::scatter(const Ray& ray_in, const SurfaceInteraction& hrec, scatter_record& srec) const {
    Vector3f reflected = reflect(ray_in.direction().normalized(), hrec.normal);
    srec.specular_ray = Ray(hrec.p, reflected + fuzz * this->random_in_unit_sphere(), ray_in.time());
    srec.attenuation = albedo;
    srec.is_specular = true;
    srec.pdf_ptr = nullptr;
    return true;
}
