//
//  aabb.hpp
//  raytracer
//
//  Created by Adrian Russell on 09/12/2018.
//  Copyright © 2018 Adrian Russell. All rights reserved.
//

#ifndef aabb_hpp
#define aabb_hpp

#include "Vector3.hpp"
#include "Ray.hpp"

inline float ffmin(float a, float b) { return a < b ? a : b; }
inline float ffmax(float a, float b) { return a > b ? a : b; }

class aabb {
public:
    aabb() {}
    aabb(const Vector3& a, const Vector3& b) { _min = a; _max = b; }
    
    Vector3 min() const { return _min; }
    Vector3 max() const { return _max; }
    
    // TODO: replace with version on next week page 11
    bool hit(const ray& r, float tmin, float tmax) const {
        {
            float t0 = ffmin((_min.x - r.origin().x) / r.direction().x,
                             (_max.x - r.origin().x) / r.direction().x);
            float t1 = ffmax((_min.x - r.origin().x) / r.direction().x,
                             (_max.x - r.origin().x) / r.direction().x);
            tmin = ffmax(t0, tmin);
            tmax = ffmin(t1, tmax);
            if (tmax <= tmin) {
                return false;
            }
        }
        {
            float t0 = ffmin((_min.y - r.origin().y) / r.direction().y,
                             (_max.y - r.origin().y) / r.direction().y);
            float t1 = ffmax((_min.y - r.origin().y) / r.direction().y,
                             (_max.y - r.origin().y) / r.direction().y);
            tmin = ffmax(t0, tmin);
            tmax = ffmin(t1, tmax);
            if (tmax <= tmin) {
                return false;
            }
        }
        {
            float t0 = ffmin((_min.z - r.origin().z) / r.direction().z,
                             (_max.z - r.origin().z) / r.direction().z);
            float t1 = ffmax((_min.z - r.origin().z) / r.direction().z,
                             (_max.z - r.origin().z) / r.direction().z);
            tmin = ffmax(t0, tmin);
            tmax = ffmin(t1, tmax);
            if (tmax <= tmin) {
                return false;
            }
        }
        return true;
    }
    
    int longest_axis() {
        Vector3 length = _max - _min;
        double max = ffmax(length.x, ffmax(length.y, length.z));
        if (max == length.x) {
            return 0;
        } else if (max == length.y) {
            return 1;
        } else {
            return 2;
        }
    }
    
    double area() {
        Vector3 length = _max - _min;
        return length.x * length.y * length.z;
    }
    
private:
    Vector3 _min;
    Vector3 _max;
};

aabb surronding_box(aabb box0, aabb box1);



#endif /* aabb_hpp */
