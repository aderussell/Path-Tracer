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

bool dielectric::scatter(const ray &ray_in, const hit_record &hrec, scatter_record& srec) const {
    srec.is_specular = true;
    srec.pdf_ptr = nullptr;
    srec.attenuation = Color(1,1,1);
    Vector3 outward_normal;
    Vector3 reflected = this->reflect(ray_in.direction(), hrec.normal);
    Vector3 refracted;
    float ni_over_nt;
    float reflect_prob;
    float cosine;
    if (Vector3::dotProduct(ray_in.direction(), hrec.normal) > 0) {
        outward_normal = hrec.normal * -1.0;
        ni_over_nt = ref_idx;
        cosine = ref_idx * Vector3::dotProduct(ray_in.direction(), hrec.normal) / ray_in.direction().length();
        
        // Beer-Lambert law
        //Color directionColor(ray_in.direction().x, ray_in.direction().y, ray_in.direction().z);
        Color absorb = hrec.t * density * volumeColor;
        Color transparency = Color(exp(-absorb.r), exp(-absorb.g), exp(-absorb.b));
        srec.attenuation *= transparency;
    } else {
        outward_normal = hrec.normal;
        ni_over_nt = 1.0 / ref_idx;
        cosine = -Vector3::dotProduct(ray_in.direction(), hrec.normal) / ray_in.direction().length();
    }
    if (refract(ray_in.direction(), outward_normal, ni_over_nt, refracted)) {
        //scattered = ray(hrec.p, refracted);
        reflect_prob = this->schlick(cosine, ref_idx);
    } else {
        //scattered = ray(hrec.p, reflected);
        reflect_prob = 1.0;
    }
    if (drand48() < reflect_prob) {
        srec.specular_ray = ray(hrec.p, reflected);
    } else {
        srec.specular_ray = ray(hrec.p, refracted);
    }
    return true;
}
