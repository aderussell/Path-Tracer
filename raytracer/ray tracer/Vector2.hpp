//
//  Vector2.hpp
//  raytracer
//
//  Created by Adrian Russell on 21/02/2019.
//  Copyright © 2019 Adrian Russell. All rights reserved.
//

#ifndef Vector2_hpp
#define Vector2_hpp


typedef Vector2<double> Vector2f;
typedef Vector2<int> Vector2i;

template <typename T>
class Vector2 {
public:
    
    Vector2() { x = y = 0.0; }
    
    Vector2(T x, T y) : x(x), y(y) {};
    
    Vector2(const Vector3f &v) : x(v.x), y(v.y), z(0) {};
    
    
    Vector2<T> &operator+= (const Vector2<T> &v) {
        x += v.x;
        y += v.y;
        return *this;
    }
    
    Vector2<T> &operator-= (const Vector2<T> &v) {
        x -= v.x;
        y -= v.y;
        return *this;
    }
    
    template <typename U>
    Vector2<T> &operator*= (U s) {
        x *= s;
        y *= s;
        return *this;
    }
    
    template <typename U>
    Vector2<T> &operator/= (U s) {
        double inv = (double)1.0 / s;
        x *= inv;
        y *= inv;
        return *this;
    }
    
    Vector2<T> operator+ (const Vector2<T> &v) const {
        return Vector2(x + v.x, y + v.y);
    }
    Vector2<T> operator- (const Vector2<T> &v) const {
        return Vector2(x - v.x, y - v.y);
    }
    
    template <typename U>
    Vector2<T> operator* (U s) const {
        return Vector2<T>(x*s, y*s);
    }
    
    template <typename U>
    Vector2<T> operator/ (U s) const {
        double inv = (double)1.0 / s;
        return Vector2<T>(x*inv, y*inv);
    }
    
    Vector2<T> operator-() const {
        return Vector2<T>(-x, -y);
    }
    
    bool operator== (const Vector2<T> &v) const {
        return ((x == v.x) && (y == v.y));
    }
    
    bool operator!= (const Vector2<T> &v) const {
        return ((x != v.x) || (y == v.y));
    }
    
    Vector2<T> inverse() const {
        return Vector2<T>(-this->x, -this->y);
    }
    
    T x, y;
    
};

#endif /* Vector2_hpp */
