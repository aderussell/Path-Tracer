//
//  dielectric.cpp
//  raytracer
//
//  Created by Adrian Russell on 07/12/2018.
//  Copyright © 2018 Adrian Russell. All rights reserved.
//

#include "dielectric.hpp"
#include <stdlib.h>

Vector3 dielectric::reflect(const Vector3& v, const Vector3& n) const {
    return v - 2 * Vector3::dotProduct(v, n) * n;
}

bool dielectric::refract(const Vector3& v, const Vector3& n, float ni_over_nt, Vector3& refracted) const {
    Vector3 uv = v.normalise();
    float dt = Vector3::dotProduct(uv, n);
    float discriminant = 1.0 - ni_over_nt*ni_over_nt*(1-dt*dt);
    if (discriminant > 0) {
        refracted = ni_over_nt * (uv - n*dt) - n*sqrt(discriminant);
        return true;
    } else {
        return false;
    }
}

float dielectric::schlick(float cosine, float ref_idx) const {
    float r0 = (1-ref_idx) / (1+ref_idx);
    r0 = r0 * r0;
    return r0 + (1-r0)*pow((1-cosine), 5);
}

bool dielectric::scatter(const ray &ray_in, const hit_record &rec, Vector3 &attenuation, ray &scattered) const {
    Vector3 outward_normal;
    Vector3 reflected = this->reflect(ray_in.direction(), rec.normal);
    float ni_over_nt;
    attenuation = Vector3(1,1,1);
    Vector3 refracted;
    float reflect_prob;
    float cosine;
    if (Vector3::dotProduct(ray_in.direction(), rec.normal) > 0) {
        outward_normal = rec.normal * -1.0;
        ni_over_nt = ref_idx;
        cosine = ref_idx * Vector3::dotProduct(ray_in.direction(), rec.normal) / ray_in.direction().length();
    } else {
        outward_normal = rec.normal;
        ni_over_nt = 1.0 / ref_idx;
        cosine = -Vector3::dotProduct(ray_in.direction(), rec.normal) / ray_in.direction().length();
    }
    if (this->refract(ray_in.direction(), outward_normal, ni_over_nt, refracted)) {
        scattered = ray(rec.p, refracted);
        reflect_prob = this->schlick(cosine, ref_idx);
    } else {
        scattered = ray(rec.p, reflected);
        reflect_prob = 1.0;
    }
    if (drand48() < reflect_prob) {
        scattered = ray(rec.p, reflected);
    } else {
        scattered = ray(rec.p, refracted);
    }
    return true;
}
