//
//  sphere.cpp
//  raytracer
//
//  Created by Adrian Russell on 06/12/2018.
//  Copyright © 2018 Adrian Russell. All rights reserved.
//

#include "sphere.hpp"

bool sphere::hit(const ray &r, float t_min, float t_max, hit_record &rec) const {
    
    Vector3 oc = r.origin() - center;
    float a = Vector3::dotProduct(r.direction(), r.direction());
    float b = Vector3::dotProduct(oc, r.direction());
    float c = Vector3::dotProduct(oc, oc) - (radius * radius);
    float discriminant = b*b - a*c;
    if (discriminant > 0) {
        float temp = (-b - sqrt(b*b-a*c))/a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.parameterAtPoint(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.mat_ptr = mat;
            return true;
        }
        temp = (-b + sqrt(b*b-a*c))/a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.parameterAtPoint(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.mat_ptr = mat;
            return true;
        }
    }
    return false;
}
