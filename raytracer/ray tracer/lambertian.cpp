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

Vector3 random_in_unit_sphere() {
    Vector3 p;
    do {
        p = 2.0 * Vector3(drand48(), drand48(), drand48()) - Vector3(1,1,1);
    } while (p.squareMagnitude() >= 1.0);
    return p;
}

inline Vector3 random_cosine_direction() {
    float r1 = drand48();
    float r2 = drand48();
    float z = sqrt(1-r2);
    float phi = 2*M_PI*r1;
    float x = cos(phi)*2*sqrt(r2);
    float y = sin(phi)*2*sqrt(r2);
    return Vector3(x,y,z);
}


bool lambertian::scatter(const ray& ray_in, const hit_record& hrec, scatter_record& srec) const {
    srec.is_specular = false;
    srec.attenuation = albedo->value(hrec.u, hrec.v, hrec.p);
    srec.pdf_ptr = new cosine_pdf(hrec.normal);
    return true;
};

float lambertian::scattering_pdf(const ray& ray_in, const hit_record& rec, ray& scattered) const {
    float cosine = Vector3::dotProduct(rec.normal, scattered.direction().normalise());
    if (cosine < 0) {
        cosine = 0;
    }
    return cosine / M_PI;
};
