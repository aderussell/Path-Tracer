//
//  sphere.cpp
//  raytracer
//
//  Created by Adrian Russell on 06/12/2018.
//  Copyright © 2018 Adrian Russell. All rights reserved.
//

#include "sphere.hpp"
#include <float.h>
#include <stdlib.h>
#include "onb.hpp"

void get_sphere_uv(const Vector3& p, float& u, float& v) {
    float phi = atan2(p.z, p.x);
    float theta = asin(p.y);
    u = 1-(phi+M_PI) / (2*M_PI);
    v = (theta+M_PI/2) / M_PI;
}

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
            get_sphere_uv((rec.p-center)/radius, rec.u, rec.v);
            return true;
        }
        temp = (-b + sqrt(b*b-a*c))/a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.parameterAtPoint(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.mat_ptr = mat;
            get_sphere_uv((rec.p-center)/radius, rec.u, rec.v);
            return true;
        }
    }
    return false;
}

bool sphere::bounding_box(float t0, float t1, aabb &box) const {
    box = aabb(center - Vector3(radius, radius, radius), center + Vector3(radius, radius, radius));
    return true;
}

float sphere::pdf_value(const Vector3 &o, const Vector3 &v) const {
    hit_record rec;
    if (this->hit(ray(o, v), 0.001, FLT_MAX, rec)) {
        float cos_theta_max = sqrt(1 - radius*radius/(center-o).squareMagnitude());
        float solid_angle = 2*M_PI*(1-cos_theta_max);
        return 1.0 / solid_angle;
    } else {
        return 0;
    }
}


inline Vector3 random_to_sphere(float radius, float distance_squared) {
    float r1 = drand48();
    float r2 = drand48();
    float z = 1 + r2*(sqrt(1-radius*radius/distance_squared) - 1);
    float phi = 2*M_PI*r1;
    float x = cos(phi)*2*sqrt(1-z*z);
    float y = sin(phi)*2*sqrt(1-z*z);
    return Vector3(x,y,z);
}

Vector3 sphere::random(const Vector3 &o) const {
    Vector3 direction = center - o;
    float distance_squared = direction.squareMagnitude();
    onb uvw;
    uvw.build_from_w(direction);
    return uvw.local(random_to_sphere(radius, distance_squared));
}




Vector3 moving_sphere::center(float time) const {
    return center0 + ((time - time0) / (time1 - time0))*(center1-center0);
}

bool moving_sphere::hit(const ray &r, float t_min, float t_max, hit_record &rec) const {
    Vector3 oc = r.origin() - center(r.time());
    float a = Vector3::dotProduct(r.direction(), r.direction());
    float b = Vector3::dotProduct(oc, r.direction());
    float c = Vector3::dotProduct(oc, oc) - (radius * radius);
    float discriminant = b*b - a*c;
    if (discriminant > 0) {
        float temp = (-b - sqrt(b*b-a*c))/a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.parameterAtPoint(rec.t);
            rec.normal = (rec.p - center(r.time())) / radius;
            rec.mat_ptr = mat;
            get_sphere_uv((rec.p-center(r.time()))/radius, rec.u, rec.v);
            return true;
        }
        temp = (-b + sqrt(b*b-a*c))/a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.parameterAtPoint(rec.t);
            rec.normal = (rec.p - center(r.time())) / radius;
            rec.mat_ptr = mat;
            get_sphere_uv((rec.p-center(r.time()))/radius, rec.u, rec.v);
            return true;
        }
    }
    return false;
}


bool moving_sphere::bounding_box(float t0, float t1, aabb &box) const {
    aabb box0 = aabb(center(t0) - Vector3(radius, radius, radius), center(t0) + Vector3(radius, radius, radius));
    aabb box1 = aabb(center(t1) - Vector3(radius, radius, radius), center(t1) + Vector3(radius, radius, radius));
    box = surronding_box(box0, box1);
    return true;
}
