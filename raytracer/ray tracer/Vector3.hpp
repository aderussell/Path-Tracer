//
//  Vector3.h
//
//  Created by Adrian Russell on 10/8/13.
//  Copyright (c) 2013 Adrian Russell. All rights reserved.
//

#ifndef __Vector3_H__
#define __Vector3_H__

#include <math.h>

struct Vector3 {
    
public:
    Vector3(double x = 0, double y = 0, double z = 0);
    ~Vector3() {};
    
    Vector3& operator+= (const Vector3 &v);
    Vector3& operator-= (const Vector3 &v);
    Vector3& operator*= (const double s);
    Vector3& operator/= (const double s);
    
    
    const Vector3 operator+ (const Vector3 &v);
    const Vector3 operator- (const Vector3 &v);
    const Vector3 operator* (const double s) const;
    
    //const Vector3 operator* (const double s, const Vector3 &v) const;
    
    Vector3 operator/ (double s);
    
    bool operator== (const Vector3 &v);
    bool operator!= (const Vector3 &v);
    
    //const Vector3 transform(const double angle);
    
    Vector3 normalise() const;
    
    double squareMagnitude() const;
    double magnitude() const;
    
    double length() const;
    
    Vector3 normal() const;
    
    static double dotProduct(Vector3 v1, Vector3 v2);
    
    //static const Vector3 vectorWithMagnitudeRotation(const double magnitude, const double angle);
    
    static Vector3 crossProduct(Vector3 v1, Vector3 v2);
    
    static Vector3 normalisedCrossProduct(Vector3 v1, Vector3 v2);
    
    static double distance(Vector3 v1, Vector3 v2);
    
    double x, y, z;
    
};

inline const Vector3 operator* (const double s, const Vector3 &v)
{
    return Vector3(v) *= s;
}

inline const Vector3 operator+ (const Vector3 &a, const Vector3 &b)
{
    return Vector3(a) += b;
}

inline const Vector3 operator- (const Vector3 &a, const Vector3 &b)
{
    return Vector3(a) -= b;
}

inline const Vector3 operator/ (const Vector3 &a, const float &b)
{
    return Vector3(a) /= b;
}

#endif /* defined(__Vector3_H__) */
