//
//  Vector3.h
//
//  Created by Adrian Russell on 10/8/13.
//  Copyright (c) 2013 Adrian Russell. All rights reserved.
//

#ifndef Vector3_hpp
#define Vector3_hpp

#include <cmath>
//#include <stdlib>

template <typename T>
class Vector3 {
private:
    T _x, _y, _z;
    
public:
    
    Vector3() { _x = _y = _z = 0; }
    
    Vector3(T x, T y, T z)  : _x(x), _y(y), _z(z) {};
    
    T x() const {
        return _x;
    }

    T y() const {
        return _y;
    }

    T z() const {
        return _z;
    }

    T &x() {
        return _x;
    }

    T &y() {
        return _y;
    }

    T &z() {
        return _z;
    }
    
    
    Vector3(const Vector3<T> &v) {
        _x = v.x();
        _y = v.y();
        _z = v.z();
    }
    
    Vector3<T> &operator+= (const Vector3<T> &v) {
        _x += v.x();
        _y += v.y();
        _z += v.z();
        return *this;
    }
    
    Vector3<T> &operator-= (const Vector3<T> &v) {
        _x -= v.x();
        _y -= v.y();
        _z -= v.z();
        return *this;
    }
    
    template <typename U>
    Vector3<T> &operator*= (U s) {
        _x *= s;
        _y *= s;
        _z *= s;
        return *this;
    }
    
    template <typename U>
    Vector3<T> &operator/= (U s) {
        T inv = (T)1.0 / s;
        _x *= inv;
        _y *= inv;
        _z *= inv;
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
        return Vector3(_x + v.x(), _y + v.y(), _z + v.z());
    }
    Vector3<T> operator- (const Vector3<T> &v) const {
        return Vector3(_x - v.x(), _y - v.y(), _z - v.z());
    }
    
    template <typename U>
    Vector3<T> operator*(U s) const {
        return Vector3<T>(_x * s, _y * s, _z * s);
    }
    
    template <typename U>
    Vector3<T> operator/ (U s) const {
        double inv = (double)1.0 / s;
        return Vector3<T>(_x*inv, _y*inv, _z*inv);
    }
    
    Vector3<T> operator-() const {
        return Vector3<T>(-_x, -_y, -_z);
    }
    
    bool operator== (const Vector3<T> &v) const {
        return ((_x == v.x) && (_y == v.y) && (_z == v.z));
    }
    
    bool operator!= (const Vector3<T> &v) const {
        return ((_x != v.x) || (_y != v.y) || (_z != v.z));
    }
    
    
    
    Vector3<T> inverse() const {
        return Vector3<T>(-_x, -_y, -_z);
    }
    
    Vector3<T> normalized() const {
        return (_x == 0.0f && _y == 0.0f && _z == 0.0f) ? Vector3(*this) : (*this * (1.0 / this->length()));
    }
    
    double squareMagnitude() const {
        return (_x * _x) + (_y * _y) + (_z * _z);
    }
    
    double magnitude() const {
        return sqrt(this->squareMagnitude());
    }
    
    double length() const {
        return this->magnitude();
    }
    
    Vector3<T> normal() const {
         return Vector3<T>(-_y, _x, _z);
    }
    
    static double dotProduct(Vector3 v1, Vector3 v2) {
        return (v1.x() * v2.x()) + (v1.y() * v2.y()) + (v1.z() * v2.z());
    }
    
    static Vector3 crossProduct(Vector3 v1, Vector3 v2) {
        return Vector3(v1.y()*v2.z() - v1.z()*v2.y(), v1.z()*v2.x() - v1.x()*v2.z(), v1.x()*v2.y() - v1.y()*v2.x());
    }
    
    static Vector3 normalisedCrossProduct(Vector3 v1, Vector3 v2) {
        return Vector3(v1.y()*v2.z() - v1.z()*v2.y(), v1.z()*v2.x() - v1.x()*v2.z(), v1.x()*v2.y() - v1.y()*v2.x()).normalized();
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
    return Vector3<T>(v.x()*s, v.y()*s, v.z()*s);
}

template <typename T>
inline const Vector3<T> operator+ (const double s, const Vector3<T> &v) {
    return Vector3<T>(v.x+s, v.y+s, v.z+s);
}

template <typename T>
inline const Vector3<T> operator+ (const Vector3<T> &v, const double s) {
    return Vector3<T>(v.x+s, v.y+s, v.z+s);
}

template <typename T>
inline Vector3<T> abs(const Vector3<T> &v) {
    return Vector3<T>(std::abs(v.x), std::abs(v.y), std::abs(v.z));
}

typedef Vector3<float> Vector3f;

#endif /* Vector3_hpp */
