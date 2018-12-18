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

//bool lambertian::scatter(const ray& ray_in, const hit_record& rec, Vector3& attenuation, ray& scattered) const {
//    Vector3 target = rec.p + rec.normal + random_in_unit_sphere();
//    scattered = ray(rec.p, target - rec.p, ray_in.time());
//    Color color = albedo->value(rec.u, rec.v, rec.p);
//    attenuation = Vector3(color.r, color.g, color.b);
//    return true;
//}

bool lambertian::scatter(const ray& ray_in, const hit_record& rec, Color& alb, ray& scattered, float& pdf) const {
    Vector3 direction;
    do {
        direction = random_in_unit_sphere();
    } while (Vector3::dotProduct(direction, rec.normal) < 0);
    //Vector3 target = rec.p + rec.normal + random_in_unit_sphere();
    scattered = ray(rec.p, (direction).normalise(), ray_in.time());
    alb = albedo->value(rec.u, rec.v, rec.p);
    pdf = 0.5 / M_PI;
    return true;
};

float lambertian::scattering_pdf(const ray& ray_in, const hit_record& rec, ray& scattered) const {
    float cosine = Vector3::dotProduct(rec.normal, scattered.direction().normalise());
    if (cosine < 0) {
        cosine = 0;
    }
    return cosine / M_PI;
};
