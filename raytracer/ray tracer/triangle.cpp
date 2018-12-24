//
//  triangle.cpp
//  raytracer
//
//  Created by Adrian Russell on 24/12/2018.
//  Copyright © 2018 Adrian Russell. All rights reserved.
//

#include <float.h>
#include <stdlib.h>
#include "triangle.hpp"
#include "Vector3.hpp"


bool triangle::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    Vector3 edge1 = b - a;
    Vector3 edge2 = c - a;
    
    Vector3 pvec = Vector3::crossProduct(r.direction(), edge2);
    
    float det = Vector3::dotProduct(edge1, pvec);
    float invdet = 1.0 / det;
    
    if (fabs(det) < 0.000001) {
        return false;
    }
    
    
    
    Vector3 tvec = r.origin() - a;
    
    double u = Vector3::dotProduct(tvec, pvec) * invdet;
    if (u < 0.0 || u > 1.0) {
        return false;
    }
    
    Vector3 qvec = Vector3::crossProduct(tvec, edge1);
    float v = Vector3::dotProduct(r.direction(), qvec) * invdet;
    if (v < 0.0 || u+v > 1.0) {
        return false;
    }
    
    float t = Vector3::dotProduct(edge2, qvec) * invdet;
    
    if (t < t_min || t > t_max) return false;
    
    //Vector3 normal = Vector3::normalisedCrossProduct(edge1, edge2);
    Vector3 normal = Vector3(0,1,0);
    
    rec.t = t;
    rec.p = r.parameterAtPoint(rec.t);
    rec.normal = normal;
    rec.mat_ptr = material;
    rec.u = u;
    rec.v = v;
    
    
    
    return true;
}

bool triangle::bounding_box(float t0, float t1, aabb &box) const {
    float minX = ffmin(a.x, ffmin(b.x, c.x));
    float minY = ffmin(a.y, ffmin(b.y, c.y));
    float minZ = ffmin(a.z, ffmin(b.z, c.z));
    Vector3 min(minX, minY, minZ);
    
    float maxX = ffmax(a.x, ffmax(b.x, c.x));
    float maxY = ffmax(a.y, ffmax(b.y, c.y));
    float maxZ = ffmax(a.z, ffmax(b.z, c.z));
    Vector3 max(maxX, maxY, maxZ);
    
    box = aabb(min, max);
    return true;
}

float triangle::pdf_value(const Vector3& o, const Vector3& v) const {
    hit_record rec;
    if (this->hit(ray(o, v), 0.001, FLT_MAX, rec)) {
        Vector3 edge1 = b - a;
        Vector3 edge2 = c - a;
        Vector3 normal = Vector3::crossProduct(edge1, edge2);
        float area = normal.length() * 0.5;
        normal = normal.normalise();
        float distance_squared = rec.t*rec.t;
        float cosine = fabs(Vector3::dotProduct(v, normal));
        return distance_squared / (cosine * area);
    } else {
        return 0;
    }
}

Vector3 triangle::random(const Vector3& o) const {
    float r1 = drand48();
    float r2 = drand48();
    float sr1 = r1*r1;
    Vector3 d = (1.0 - sr1)*a + sr1*(1.0 - r2)*b + sr1*r2*c;
    return d - o;
}
