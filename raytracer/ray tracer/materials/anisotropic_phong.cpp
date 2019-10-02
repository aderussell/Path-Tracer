//
//  anisotropic_phong.cpp
//  raytracer
//
//  Created by Adrian Russell on 24/01/2019.
//  Copyright © 2019 Adrian Russell. All rights reserved.
//

#include "anisotropic_phong.hpp"

Vector3f anisotropic_phong::random_in_unit_sphere() const {
    Vector3f p;
    do {
        p = 2.0 * Vector3f(drand48(), drand48(), drand48()) - Vector3f(1,1,1);
    } while (p.squareMagnitude() >= 1.0);
    return p;
}

bool anisotropic_phong::scatter(const Ray& r_in, const SurfaceInteraction& rec, scatter_record& srec) const {
    srec.is_specular = true;
    srec.attenuation = albedo->value(rec.u, rec.v, rec.p);
    srec.specular    = specular->value(rec.u, rec.v, rec.p);
    
    srec.pdf_ptr = new anisotropic_phong_pdf(r_in.direction(), rec.normal, nu, nv);
    
    if (srec.is_specular) {
        Vector3f dir = srec.pdf_ptr->generate();
        while (Vector3f::dotProduct(rec.normal, dir) < 0) {
            dir = srec.pdf_ptr->generate();
        }
        float f = Vector3f::dotProduct((rec.p + Vector3f(0.0001, 0.0001, 0.0001)), rec.normal);
        srec.specular_ray = Ray(Vector3f(f,f,f), dir, r_in.time());
    }
    
    return true;
}


float anisotropic_phong::scattering_pdf(const Ray& ray_in, const SurfaceInteraction& rec, Ray& scattered) const {
    float cosine = Vector3f::dotProduct(rec.normal, scattered.direction().normalized());
    if (cosine < 0) {
        cosine = 0;
    }
    return cosine / M_PI;
}
