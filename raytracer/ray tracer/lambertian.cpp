//
//  lambertian.cpp
//  raytracer
//
//  Created by Adrian Russell on 07/12/2018.
//  Copyright © 2018 Adrian Russell. All rights reserved.
//

#include "lambertian.hpp"
#include "onb.hpp"
#include <stdlib.h>

Vector3f random_in_unit_sphere() {
    Vector3f p;
    do {
        p = 2.0 * Vector3f(drand48(), drand48(), drand48()) - Vector3f(1,1,1);
    } while (p.squareMagnitude() >= 1.0);
    return p;
}

inline Vector3f random_cosine_direction() {
    float r1 = drand48();
    float r2 = drand48();
    float z = sqrt(1-r2);
    float phi = 2*M_PI*r1;
    float x = cos(phi)*2*sqrt(r2);
    float y = sin(phi)*2*sqrt(r2);
    return Vector3f(x,y,z);
}


bool lambertian::scatter(const ray& ray_in, const hit_record& hrec, scatter_record& srec) const {
    srec.is_specular = false;
    srec.attenuation = albedo->value(hrec.u, hrec.v, hrec.p);
    srec.pdf_ptr = new cosine_pdf(hrec.normal);
    return true;
};

float lambertian::scattering_pdf(const ray& ray_in, const hit_record& rec, ray& scattered) const {
    float cosine = Vector3f::dotProduct(rec.normal, scattered.direction().normalized());
    if (cosine < 0) {
        cosine = 0;
    }
    return cosine / M_PI;
};
