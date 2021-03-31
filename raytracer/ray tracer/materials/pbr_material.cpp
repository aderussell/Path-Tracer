//
//  pbr_material.cpp
//  raytracer
//
//  Created by Adrian Russell on 08/06/2020.
//  Copyright © 2020 Adrian Russell. All rights reserved.
//

#include "pbr_material.hpp"

Color mix(Color a, Color b, float t) {
    float x = (1.0f - t) * a.r() + t * b.r();
    float y = (1.0f - t) * a.g() + t * b.g();
    float z = (1.0f - t) * a.b() + t * b.b();
    return Color(x,y,z);
}

Vector3f pbr_material::reflect(const Vector3f& v, const Vector3f& n) const {
    return v - 2 * Vector3f::dotProduct(v, n) * n;
}

Vector3f pbr_material::random_in_unit_sphere() const {
    Vector3f p;
    do {
        p = 2.0 * Vector3f(drand48(), drand48(), drand48()) - Vector3f(1,1,1);
    } while (p.squareMagnitude() >= 1.0);
    return p;
}

bool pbr_material::scatter(const Ray& ray_in, const SurfaceInteraction& hrec, scatter_record& srec) const {
    bool doSpecular = (drand48() <= percentSpecular);
    float spec = doSpecular ? 1.0 : 0.0;
    
    if (doSpecular) {
        Vector3f reflected = reflect(ray_in.direction().normalized(), hrec.normal);
        srec.specular_ray = Ray(hrec.p, reflected + roughness * this->random_in_unit_sphere(), ray_in.time());
        srec.specular = specularColor;
    }
    srec.attenuation = mix(albedo, specularColor, spec);
    srec.is_specular = doSpecular;
    srec.pdf_ptr = new cosine_pdf(hrec.normal);
    
    return true;
}

float pbr_material::scattering_pdf(const Ray& ray_in, const SurfaceInteraction& rec, Ray& scattered) const {
    float cosine = Vector3f::dotProduct(rec.normal, scattered.direction().normalized());
    if (cosine < 0) {
        cosine = 0;
    }
    return cosine / M_PI;
};
