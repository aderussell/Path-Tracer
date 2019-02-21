//
//  Vector4.h
//  Drawing
//
//  Created by Adrian Russell on 20/01/2014.
//  Copyright (c) 2014 Adrian Russell. All rights reserved.
//

#ifndef __Drawing__Vector4__
#define __Drawing__Vector4__

#include "Matrix44.hpp"

typedef Vector4<double> Vector4f;
typedef Vector4<int> Vector4i;

template <typename T>
class Vector4 {
public:
    Vector4() { x = y = z = w = 0.0; }
    
    Vector4(T x, T y, T z, T w = 0) : x(x), y(y), z(z), w(w) {};
    
    //Vector4(const Vector3f &v) : x(v.x), y(v.y), z(v.z), w(0) {};
    
    
    Vector4<T> &operator+= (const Vector4<T> &v) {
        x += v.x;
        y += v.y;
        z += v.z;
        w += v.w;
        return *this;
    }
    
    Vector4<T> &operator-= (const Vector4<T> &v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        w -= v.w;
        return *this;
    }
    
    template <typename U>
    Vector4<T> &operator*= (U s) {
        x *= s;
        y *= s;
        z *= s;
        w *= s;
        return *this;
    }
    
    template <typename U>
    Vector4<T> &operator/= (U s) {
        double inv = (double)1.0 / s;
        x *= inv;
        y *= inv;
        z *= inv;
        w *= inv;
        return *this;
    }
    
    Vector4<T> operator+ (const Vector4<T> &v) const {
        return Vector4(x + v.x, y + v.y, z + v.z, w + v.w);
    }
    Vector4<T> operator- (const Vector4<T> &v) const {
        return Vector4(x - v.x, y - v.y, z - v.z, w - v.w);
    }
    
    template <typename U>
    Vector4<T> operator* (U s) const {
        return Vector4<T>(x*s, y*s, z*s, w*s);
    }
    
    template <typename U>
    Vector4<T> operator/ (U s) const {
        double inv = (double)1.0 / s;
        return Vector4<T>(x*inv, y*inv, z*inv, w*inv);
    }
    
    Vector4<T> operator-() const {
        return Vector4<T>(-x, -y, -z, -w);
    }
    
    bool operator== (const Vector4<T> &v) const {
        return ((x == v.x) && (y == v.y) && (z == v.z) && (w == v.w));
    }
    
    bool operator!= (const Vector4<T> &v) const {
        return ((x != v.x) || (y == v.y) || (z != v.z) || (w != v.w));
    }
    
    Vector4<T> inverse() const {
        return Vector4<T>(-this->x, -this->y, -this->z, -this->w);
    }
    
    Vector4<T> normalized() const {
        if (w == 0.0) return Vector4();
        return Vector4<T>(x/w, y/w, z/w, 1.0);
    }
    
    
    Vector4 transform(const Matrix44 &m) const {
        double x = (this->x * m.u.m11) + (this->y * m.u.m21) + (this->z * m.u.m31) + (this->w * m.u.m41);
        double y = (this->x * m.u.m12) + (this->y * m.u.m22) + (this->z * m.u.m32) + (this->w * m.u.m42);
        double z = (this->x * m.u.m13) + (this->y * m.u.m23) + (this->z * m.u.m33) + (this->w * m.u.m43);
        double w = (this->x * m.u.m14) + (this->y * m.u.m24) + (this->z * m.u.m34) + (this->w * m.u.m44);
        return Vector4(x, y, z, w);
    }
    
    
    T x, y, z, w;
};

#endif /* defined(__Drawing__Vector4__) */
