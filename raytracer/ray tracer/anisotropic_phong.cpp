//
//  anisotropic_phong.cpp
//  raytracer
//
//  Created by Adrian Russell on 24/01/2019.
//  Copyright © 2019 Adrian Russell. All rights reserved.
//

#include "anisotropic_phong.hpp"

Vector3 anisotropic_phong::random_in_unit_sphere() const {
    Vector3 p;
    do {
        p = 2.0 * Vector3(drand48(), drand48(), drand48()) - Vector3(1,1,1);
    } while (p.squareMagnitude() >= 1.0);
    return p;
}

bool anisotropic_phong::scatter(const ray& r_in, const hit_record& rec, scatter_record& srec) const {
    srec.is_specular = true;
    srec.attenuation = albedo->value(rec.u, rec.v, rec.p);
    srec.specular    = specular->value(rec.u, rec.v, rec.p);
    
    srec.pdf_ptr = new anisotropic_phong_pdf(r_in.direction(), rec.normal, nu, nv);
    
    if (srec.is_specular) {
        Vector3 dir = srec.pdf_ptr->generate();
        while (Vector3::dotProduct(rec.normal, dir) < 0) {
            dir = srec.pdf_ptr->generate();
        }
        srec.specular_ray = ray((rec.p + 0.0001) * rec.normal, dir, r_in.time());
    }
    
    return true;
}


float anisotropic_phong::scattering_pdf(const ray& ray_in, const hit_record& rec, ray& scattered) const {
    float cosine = Vector3::dotProduct(rec.normal, scattered.direction().normalise());
    if (cosine < 0) {
        cosine = 0;
    }
    return cosine / M_PI;
}
