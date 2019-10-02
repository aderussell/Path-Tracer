//
//  xy_rect.cpp
//  raytracer
//
//  Created by Adrian Russell on 13/12/2018.
//  Copyright © 2018 Adrian Russell. All rights reserved.
//

#include "xy_rect.hpp"
#include <stdlib.h>
#include <float.h>

bool xy_rect::hit(const Ray& r, float t_min, float t_max, SurfaceInteraction& rec) const {
    float t = (k-r.origin().z()) / r.direction().z();
    if (t < t_min || t > t_max) {
        return false;
    }
    float x = r.origin().x() + t*r.direction().x();
    float y = r.origin().y() + t*r.direction().y();
    if (x<x0 || x>x1 || y<y0 || y>y1) {
        return false;
    }
    rec.u = (x-x0)/(x1-x0);
    rec.v = (y-y0)/(y1-y0);
    rec.t = t;
    rec.mat_ptr = mp;
    rec.p = r.pointAtParameter(t);
    rec.normal = Vector3f(0,0,1);
    return true;
}

bool xz_rect::hit(const Ray& r, float t0, float t1, SurfaceInteraction& rec) const {
    float t = (k-r.origin().y()) / r.direction().y();
    if (t < t0 || t > t1)
        return false;
    float x = r.origin().x() + t*r.direction().x();
    float z = r.origin().z() + t*r.direction().z();
    if (x < x0 || x > x1 || z < z0 || z > z1)
        return false;
    rec.u = (x-x0)/(x1-x0);
    rec.v = (z-z0)/(z1-z0);
    rec.t = t;
    rec.mat_ptr = mp;
    rec.p = r.pointAtParameter(t);
    rec.normal = Vector3f(0, 1, 0);
    return true;
}


float xz_rect::pdf_value(const Vector3f &o, const Vector3f &v) const {
    SurfaceInteraction rec;
    if (this->hit(Ray(o,v), 0.001, FLT_MAX, rec)) {
        float area = (x1-x0)*(z1-z0);
        float distance_squared = rec.t*rec.t * v.squareMagnitude();
        float cosine = fabs(Vector3f::dotProduct(v, rec.normal) / v.length());
        return distance_squared / (cosine * area);
    } else {
        return 0;
    }
}

Vector3f xz_rect::random(const Vector3f &o) const {
    Vector3f randomPoint = Vector3f(x0 + drand48()*(x1-x0), k, z0 + drand48()*(z1-z0));
    return randomPoint - o;
}




bool yz_rect::hit(const Ray& r, float t0, float t1, SurfaceInteraction& rec) const {
    float t = (k-r.origin().x()) / r.direction().x();
    if (t < t0 || t > t1)
        return false;
    float y = r.origin().y() + t*r.direction().y();
    float z = r.origin().z() + t*r.direction().z();
    if (y < y0 || y > y1 || z < z0 || z > z1)
        return false;
    rec.u = (y-y0)/(y1-y0);
    rec.v = (z-z0)/(z1-z0);
    rec.t = t;
    rec.mat_ptr = mp;
    rec.p = r.pointAtParameter(t);
    rec.normal = Vector3f(1, 0, 0);
    return true;
}
