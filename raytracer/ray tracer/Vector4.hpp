//
//  Vector4.h
//  Drawing
//
//  Created by Adrian Russell on 20/01/2014.
//  Copyright (c) 2014 Adrian Russell. All rights reserved.
//

#ifndef __Drawing__Vector4__
#define __Drawing__Vector4__

#include <math.h>
#include "Vector3.h"
#include "Matrix44.h"

struct Vector4 {
    
public:
    Vector4(double x = 0, double y = 0, double z = 0, double w = 0);
    Vector4(Vector3 v);
    
    Vector4 operator+= (Vector4 v);
    Vector4 operator-= (Vector4 v);
    Vector4 operator*= (double s);
    Vector4 operator/= (double s);
    
    Vector4 operator+ (Vector4 v);
    Vector4 operator- (Vector4 v);
    Vector4 operator* (double s);
    Vector4 operator/ (double s);
    
    Vector4 operator- ();
    
    bool operator== (Vector4 &v);
    bool operator!= (Vector4 &v);
    
    Vector4 inverse();
    
    Vector4 normaliseScale();
    
    //const Vector3 transform(const double angle);
    
    //const Vector4 normalise();
    
    //double squareMagnitude();
    //double magnitude();
    
    //double length();
    
    //static const double dotProduct(const Vector4 &v1, const Vector4 &v2);
    
    //static const Vector3 vectorWithMagnitudeRotation(const double magnitude, const double angle);
    
    //static const Vector3 crossProduct(const Vector4 &v1, const Vector4 &v2);
    
    static const double distance(const Vector4 &v1, const Vector4 &v2);
    
    static const Vector4 Vector4Zero;
    
    static const Vector4 Vector4NoPoint;
    
    Vector4 transform(const Matrix44 &m) const;
    
    double x, y, z, w;
    
};

#endif /* defined(__Drawing__Vector4__) */
