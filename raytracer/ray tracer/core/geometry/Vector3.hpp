//
//  Vector3.h
//
//  Created by Adrian Russell on 10/8/13.
//  Copyright (c) 2013 Adrian Russell. All rights reserved.
//

#ifndef Vector3_hpp
#define Vector3_hpp

#include <cmath>
#include <x86intrin.h>
//#include <stdlib>

template <typename T>
class Vector3 {
public:
    //T _x, _y, _z;
    
    union {
        __m128 _a;
        float _f[4];
        uint32_t _i[4];
    };
    
    
    Vector3(): _a(_mm_set1_ps(0)) { }
    
    Vector3(const __m128 &a) : _a(a) {}
    
    Vector3(T x, T y, T z) : _a(_mm_set_ps(0, z, y, x)) {}
    
    T x() const {
        return _f[0];
    }

    T y() const {
        return _f[1];
    }

    T z() const {
        return _f[2];
    }

    T &x() {
        return _f[0];
    }

    T &y() {
        return _f[1];
    }

    T &z() {
        return _f[2];
    }
    
    
    Vector3(const Vector3<T> &v) {
        _a = v._a;
    }
    
    Vector3<T> &operator+= (const Vector3<T> &v) {
        _a = _mm_add_ps(_a, v._a);
        return *this;
    }
    
    Vector3<T> &operator-= (const Vector3<T> &v) {
        _a = _mm_sub_ps(_a, v._a);
        return *this;
    }
    
    template <typename U>
    Vector3<T> &operator*= (U s) {
        __m128 v = _mm_set1_ps(s);
        _a = _mm_mul_ps(_a, v);
        return *this;
    }
    
    template <typename U>
    Vector3<T> &operator/= (U s) {
        __m128 v = _mm_set1_ps(s);
        _a = _mm_div_ps(_a, v);
        return *this;
    }
    
    T& operator[] (const int index) {
        if (index == 0) {
            return x();
        } else if (index == 1) {
            return y();
        } else {
            return z();
        }
    }
    
    const T& operator[] (const int index) const {
        if (index == 0) {
            return x();
        } else if (index == 1) {
            return y();
        } else {
            return z();
        }
    }
    
    
    
    Vector3<T> operator+ (const Vector3<T> &v) const {
        return _mm_add_ps(_a, v._a);
    }
    Vector3<T> operator- (const Vector3<T> &v) const {
        return _mm_sub_ps(_a, v._a);
    }
    
    template <typename U>
    Vector3<T> operator*(U s) const {
        __m128 v = _mm_set1_ps(s);
        return _mm_mul_ps(_a, v);
    }
    
    template <typename U>
    Vector3<T> operator/ (U s) const {
        __m128 v = _mm_set1_ps(s);
        return _mm_div_ps(_a, v);
    }
    
    Vector3<T> operator-() const {
        static const __m128 SIGNMASK = _mm_castsi128_ps(_mm_set1_epi32(0x80000000));
        return _mm_xor_ps(_a, SIGNMASK);
        //return Vector3<T>(-x(), -y(), -z());
    }
    
    bool operator== (const Vector3<T> &v) const {
        return ((x() == v.x) && (y() == v.y) && (z() == v.z));
    }
    
    bool operator!= (const Vector3<T> &v) const {
        return ((x() != v.x()) || (y() != v.y()) || (z() != v.z()));
    }
    
    
    
    Vector3<T> inverse() const {
        static const __m128 SIGNMASK = _mm_castsi128_ps(_mm_set1_epi32(0x80000000));
        return _mm_xor_ps(_a, SIGNMASK);
        //return Vector3<T>(-x(), -y(), -z());
    }
    
    Vector3<T> normalized() const {
        return (x() == 0.0f && y() == 0.0f && z() == 0.0f) ? Vector3(*this) : (*this * (1.0 / this->length()));
    }
    
    double squareMagnitude() const {
        __m128 tmp = _mm_mul_ps(_a, _a);
        __m128 tmp2 = _mm_hadd_ps(tmp, tmp);
        return _mm_cvtss_f32(_mm_hadd_ps(tmp2, tmp2));
    }
    
    double magnitude() const {
        return sqrt(this->squareMagnitude());
    }
    
    double length() const {
        return this->magnitude();
    }
    
    Vector3<T> normal() const {
         return Vector3<T>(-y(), x(), z());
    }
    
    static double dotProduct(Vector3 v1, Vector3 v2) {
        __m128 tmp = _mm_mul_ps(v1._a, v2._a);
        __m128 tmp2 = _mm_hadd_ps(tmp, tmp);
        return _mm_cvtss_f32(_mm_hadd_ps(tmp2, tmp2));
        //return (v1.x() * v2.x()) + (v1.y() * v2.y()) + (v1.z() * v2.z());
    }
    
    static Vector3 crossProduct(Vector3 v1, Vector3 v2) {
        __m128 x = v1._a;
        __m128 y = v2._a;
        __m128 tmp0 = _mm_shuffle_ps(y,y,_MM_SHUFFLE(3,0,2,1));
        __m128 tmp1 = _mm_shuffle_ps(x,x,_MM_SHUFFLE(3,0,2,1));
        tmp1 = _mm_mul_ps(tmp1,y);
        __m128 tmp2 = _mm_fmsub_ps( tmp0,x, tmp1 );
        return _mm_shuffle_ps(tmp2,tmp2,_MM_SHUFFLE(3,0,2,1));
        //return Vector3(v1.y()*v2.z() - v1.z()*v2.y(), v1.z()*v2.x() - v1.x()*v2.z(), v1.x()*v2.y() - v1.y()*v2.x());
    }
    
    static Vector3 normalisedCrossProduct(Vector3 v1, Vector3 v2) {
        return crossProduct(v1, v2).normalized();
    }
    
    static double distance(Vector3 v1, Vector3 v2) {
        double distx = (v2.x - v1.x);
        double disty = (v2.y - v1.y);
        double distz = (v2.z - v1.z);
        return sqrt( (distx * distx) + (disty * disty) + (distz * distz) );
    }
};

template <typename T>
inline const Vector3<T> operator* (const double s, const Vector3<T> &v) {
    __m128 t = _mm_set1_ps(s);
    return _mm_mul_ps(v._a, t);
    //return Vector3<T>(v.x()*s, v.y()*s, v.z()*s);
}

template <typename T>
inline const Vector3<T> operator+ (const double s, const Vector3<T> &v) {
    __m128 t = _mm_set1_ps(s);
    return _mm_add_ps(v._a, t);
    //return Vector3<T>(v.x+s, v.y+s, v.z+s);
}

template <typename T>
inline const Vector3<T> operator+ (const Vector3<T> &v, const double s) {
    __m128 t = _mm_set1_ps(s);
    return _mm_add_ps(v._a, t);
    //return Vector3<T>(v.x+s, v.y+s, v.z+s);
}

template <typename T>
inline Vector3<T> abs(const Vector3<T> &v) {
    static const __m128 SIGNMASK = _mm_castsi128_ps(_mm_set1_epi32(0x80000000));
    return _mm_andnot_ps(SIGNMASK, v._a);
}

typedef Vector3<float> Vector3f;

#endif /* Vector3_hpp */
