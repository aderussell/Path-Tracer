//
//  Vector3.h
//
//  Created by Adrian Russell on 10/8/13.
//  Copyright (c) 2013 Adrian Russell. All rights reserved.
//

#ifndef Vector3_hpp
#define Vector3_hpp

#include <math.h>

template <typename T>
class Vector3 {
public:
    
    Vector3() { x = y = z = 0; }
    
    Vector3(T x, T y, T z)  : x(x), y(y), z(z) {};
    
    Vector3(const Vector3<T> &v) {
        x = v.x;
        y = v.y;
        z = v.z;
    }
    
    Vector3<T> &operator+= (const Vector3<T> &v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }
    
    Vector3<T> &operator-= (const Vector3<T> &v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }
    
    template <typename U>
    Vector3<T> &operator*= (U s) {
        x *= s;
        y *= s;
        z *= s;
        return *this;
    }
    
    template <typename U>
    Vector3<T> &operator/= (U s) {
        double inv = (double)1.0 / s;
        x *= inv;
        y *= inv;
        z *= inv;
        return *this;
    }
    
    T& operator[] (const int index) {
        if (index == 0) {
            return x;
        } else if (index == 1) {
            return y;
        } else {
            return z;
        }
    }
    
    const T& operator[] (const int index) const {
        if (index == 0) {
            return x;
        } else if (index == 1) {
            return y;
        } else {
            return z;
        }
    }
    
    
    
    Vector3<T> operator+ (const Vector3<T> &v) const {
        return Vector3(x + v.x, y + v.y, z + v.z);
    }
    Vector3<T> operator- (const Vector3<T> &v) const {
        return Vector3(x - v.x, y - v.y, z - v.z);
    }
    
    template <typename U>
    Vector3<T> operator*(U s) const {
        return Vector3<T>(x * s, y * s, z * s);
    }
    
    template <typename U>
    Vector3<T> operator/ (U s) const {
        double inv = (double)1.0 / s;
        return Vector3<T>(x*inv, y*inv, z*inv);
    }
    
    Vector3<T> operator-() const {
        return Vector3<T>(-x, -y, -z);
    }
    
    bool operator== (const Vector3<T> &v) const {
        return ((x == v.x) && (y == v.y) && (z == v.z));
    }
    
    bool operator!= (const Vector3<T> &v) const {
        return ((x != v.x) || (y != v.y) || (z != v.z));
    }
    
    
    
    Vector3<T> inverse() const {
        return Vector3<T>(-x, -y, -z);
    }
    
    Vector3<T> normalized() const {
        return (x == 0.0f && y == 0.0f && z == 0.0f) ? Vector3(*this) : (*this * (1.0 / this->length()));
    }
    
    double squareMagnitude() const {
        return (x * x) + (y * y) + (z * z);
    }
    
    double magnitude() const {
        return sqrt(this->squareMagnitude());
    }
    
    double length() const {
        return this->magnitude();
    }
    
    Vector3<T> normal() const {
         return Vector3<T>(-y, x, z);
    }
    
    static double dotProduct(Vector3 v1, Vector3 v2) {
        return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
    }
    
    static Vector3 crossProduct(Vector3 v1, Vector3 v2) {
        return Vector3(v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x);
    }
    
    static Vector3 normalisedCrossProduct(Vector3 v1, Vector3 v2) {
        return Vector3(v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x).normalized();
    }
    
    static double distance(Vector3 v1, Vector3 v2) {
        double distx = (v2.x - v1.x);
        double disty = (v2.y - v1.y);
        double distz = (v2.z - v1.z);
        return sqrt( (distx * distx) + (disty * disty) + (distz * distz) );
    }

    
    
    T x, y, z;
};

template <typename T>
inline const Vector3<T> operator* (const double s, const Vector3<T> &v) {
    return Vector3<T>(v.x*s, v.y*s, v.z*s);
}

template <typename T>
inline const Vector3<T> operator+ (const double s, const Vector3<T> &v) {
    return Vector3<T>(v.x+s, v.y+s, v.z+s);
}

template <typename T>
inline const Vector3<T> operator+ (const Vector3<T> &v, const double s) {
    return Vector3<T>(v.x+s, v.y+s, v.z+s);
}

typedef Vector3<double> Vector3f;

#endif /* Vector3_hpp */
