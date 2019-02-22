//
//  xy_rect.hpp
//  raytracer
//
//  Created by Adrian Russell on 13/12/2018.
//  Copyright © 2018 Adrian Russell. All rights reserved.
//

#ifndef xy_rect_hpp
#define xy_rect_hpp

#include "hitable.hpp"
#include "Vector3.hpp"

class xy_rect: public hitable {
public:
    xy_rect() {}
    xy_rect(float _x0, float _x1, float _y0, float _y1, float _k, material *m) : x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), mp(m) {}
    
    virtual bool hit(const Ray&r, float t_min, float t_max, hit_record& rec) const;
    virtual bool bounding_box(float t0, float t1, aabb& box) const {
        box = aabb(Vector3f(x0,y0,k-0.0001), Vector3f(x1,y1,k+0.0001));
        return true;
    }
    
private:
    material *mp;
    float x0, x1, y0, y1, k;
};


class xz_rect: public hitable  {
public:
    xz_rect() {}
    xz_rect(float _x0, float _x1, float _z0, float _z1, float _k, material *mat) : x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), mp(mat) {};
    virtual bool hit(const Ray& r, float t0, float t1, hit_record& rec) const;
    virtual bool bounding_box(float t0, float t1, aabb& box) const {
        box =  aabb(Vector3f(x0,k-0.0001,z0), Vector3f(x1, k+0.0001, z1));
        return true; }
    
    float pdf_value(const Vector3f &o, const Vector3f &v) const;
    Vector3f random(const Vector3f &o) const;
    
private:
    material  *mp;
    float x0, x1, z0, z1, k;
};

class yz_rect: public hitable  {
public:
    yz_rect() {}
    yz_rect(float _y0, float _y1, float _z0, float _z1, float _k, material *mat) : y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k), mp(mat) {};
    virtual bool hit(const Ray& r, float t0, float t1, hit_record& rec) const;
    virtual bool bounding_box(float t0, float t1, aabb& box) const {
        box =  aabb(Vector3f(k-0.0001, y0, z0), Vector3f(k+0.0001, y1, z1));
        return true; }
    
private:
    material  *mp;
    float y0, y1, z0, z1, k;
};

#endif /* xy_rect_hpp */
