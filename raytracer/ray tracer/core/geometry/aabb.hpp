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
#include "ray.hpp"
#include <x86intrin.h>

template <typename T>
inline T ffmin(T a, T b) { return a < b ? a : b; }
template <typename T>
inline T ffmax(T a, T b) { return a > b ? a : b; }
template <typename T>
inline T ffclamp(T a, T min, T max) { return ffmax(min, ffmin(max, a)); }

class aabb {
public:
    aabb() {}
    aabb(const Vector3f& a, const Vector3f& b) { _min = a; _max = b; }
    
    Vector3f min() const { return _min; }
    Vector3f max() const { return _max; }

    
//    bool hit(const Ray& r, float _tmin, float _tmax) const {
//        __m128 min = _mm_div_ps(_mm_sub_ps(_min._a, r.origin()._a), r.direction()._a);
//        __m128 max = _mm_div_ps(_mm_sub_ps(_max._a, r.origin()._a), r.direction()._a);
//        __m128 t0  = _mm_min_ps(min, max);
//        __m128 t1  = _mm_max_ps(min, max);
//        __m128 tmin1 = _mm_set1_ps(_tmin);
//        __m128 tmax1 = _mm_set1_ps(_tmax);
//        __m128 tmin = _mm_max_ps(t0, tmin1);
//        __m128 tmax = _mm_min_ps(t1, tmax1);
//        __m128 z = _mm_cmple_ps(tmax, tmin);
//        return !(_mm_movemask_ps(z) != 0);
//    }

    bool hit(const Ray& r, float tmin, float tmax) const {

        Vector3f min = _mm_div_ps(_mm_sub_ps(_min._a, r.origin()._a), r.direction()._a);
        Vector3f max = _mm_div_ps(_mm_sub_ps(_max._a, r.origin()._a), r.direction()._a);
//                __m128 _t0  = _mm_min_ps(min._a, max._a);
//                __m128 _t1  = _mm_max_ps(min._a, max._a);
//                __m128 tmin1 = _mm_set1_ps(tmin);
//                __m128 tmax1 = _mm_set1_ps(tmax);
//                __m128 _tmin = _mm_max_ps(_t0, tmin1);
//                __m128 _tmax = _mm_min_ps(_t1, tmax1);
//                __m128 z = _mm_cmple_ps(_tmax, _tmin);
//                bool b = !(_mm_movemask_ps(z) != 0);
//        return b;
        {

            float t0 = ffmin(min.x(),
                             max.x());
            float t1 = ffmax(min.x(),
                             max.x());
            tmin = ffmax(t0, tmin);
            tmax = ffmin(t1, tmax);
            if (tmax <= tmin) {
                return false;
            }
        }
        {
            float t0 = ffmin(min.y(),
                             max.y());
            float t1 = ffmax(min.y(),
                             max.y());
            tmin = ffmax(t0, tmin);
            tmax = ffmin(t1, tmax);
            if (tmax <= tmin) {
                return false;
            }
        }
        {
            float t0 = ffmin(min.z(),
                             max.z());
            float t1 = ffmax(min.z(),
                             max.z());
            tmin = ffmax(t0, tmin);
            tmax = ffmin(t1, tmax);
            if (tmax <= tmin) {
                return false;
            }
        }
        return true;
    }
    
//    bool hit(const Ray& r, float tmin, float tmax) const {
//        {
//            float t0 = ffmin((_min.x() - r.origin().x()) / r.direction().x(),
//                             (_max.x() - r.origin().x()) / r.direction().x());
//            float t1 = ffmax((_min.x() - r.origin().x()) / r.direction().x(),
//                             (_max.x() - r.origin().x()) / r.direction().x());
//            tmin = ffmax(t0, tmin);
//            tmax = ffmin(t1, tmax);
//            if (tmax <= tmin) {
//                return false;
//            }
//        }
//        {
//            float t0 = ffmin((_min.y() - r.origin().y()) / r.direction().y(),
//                             (_max.y() - r.origin().y()) / r.direction().y());
//            float t1 = ffmax((_min.y() - r.origin().y()) / r.direction().y(),
//                             (_max.y() - r.origin().y()) / r.direction().y());
//            tmin = ffmax(t0, tmin);
//            tmax = ffmin(t1, tmax);
//            if (tmax <= tmin) {
//                return false;
//            }
//        }
//        {
//            float t0 = ffmin((_min.z() - r.origin().z()) / r.direction().z(),
//                             (_max.z() - r.origin().z()) / r.direction().z());
//            float t1 = ffmax((_min.z() - r.origin().z()) / r.direction().z(),
//                             (_max.z() - r.origin().z()) / r.direction().z());
//            tmin = ffmax(t0, tmin);
//            tmax = ffmin(t1, tmax);
//            if (tmax <= tmin) {
//                return false;
//            }
//        }
//        return true;
//    }
    
    int longest_axis() {
        Vector3f length = _max - _min;
        double max = ffmax(length.x(), ffmax(length.y(), length.z()));
        if (max == length.x()) {
            return 0;
        } else if (max == length.y()) {
            return 1;
        } else {
            return 2;
        }
    }
    
    double area() {
        Vector3f length = _max - _min;
        return length.x() * length.y() * length.z();
    }
    
private:
    Vector3f _min;
    Vector3f _max;
};

aabb surrounding_box(aabb box0, aabb box1);



#endif /* aabb_hpp */
