//
//  Matrix44.h
//
//  Created by Adrian Russell on 10/8/13.
//  Copyright (c) 2013 Adrian Russell. All rights reserved.
//

#ifndef __Matrix44_H__
#define __Matrix44_H__
#include "Vector3.h"


struct Matrix44 {
    
public:
    
    Matrix44(double m11 = 0, double m12 = 0, double m13 = 0, double m14 = 0,
             double m21 = 0, double m22 = 0, double m23 = 0, double m24 = 0,
             double m31 = 0, double m32 = 0, double m33 = 0, double m34 = 0,
             double m41 = 0, double m42 = 0, double m43 = 0, double m44 = 0);
    
    
    Matrix44(double v[4][4]);
    
    Matrix44(double a[16]);
    
    // add matrix
    Matrix44 operator+ (Matrix44 m);
    
    // subtract matrix
    Matrix44 operator- (Matrix44 m);
    
    // mutliple matrix
    Matrix44 operator* (Matrix44 m);
    
    // scale matrix
    Matrix44 operator* (double s);
    
    bool operator!= (Matrix44 &m);
    
    bool operator== (Matrix44 &m);
    
    // calculate the inverse of matrix
    Matrix44 inverse();
    
    // calculate transpose of matrix;
    Matrix44 transpose();
    
    // ceck if matrix is empty... all values == 0.0
    bool isEmpty();
    
    // return an empty matrix. this is the equivilant of using Matrix44()
    static Matrix44 emptyMatrix();
    
    // return an identity matrix
    static Matrix44 indentityMatrix();
    
    // return a translation matrix with translation for specified values.
    static Matrix44 translate(double tx, double ty, double tz);
    
    // return a scale matrix with scale valyes for the specified values.
    static Matrix44 scale(double sx, double sy, double sz);
    
    static Matrix44 perspective(double f);
    
    static Matrix44 perspective(double f, double n);
    
    static Matrix44 mtxLoadPerspective(double fov, double aspect, double nearZ, double farZ);
    
    static Matrix44 Orthographic(double left, double right, double bottom, double top, double nearZ, double farZ);
    
    static Matrix44 lookAtLH(Vector3 position, Vector3 target, Vector3 upVector);
    
    static Matrix44 rotationX(double a);
    
    
    static Matrix44 rotationY(double a);
    
    
    static Matrix44 rotationZ(double a);
    
    // return a matrix with rotation values corresponding to rotating yaw, pitch & roll by the specified values.
    static Matrix44 rotationYawPitchRoll(double y, double p, double r);
    
    
    
    union {
        double v[4][4];
        double a[16];
        struct {
            double m11, m12, m13, m14,
                    m21, m22, m23, m24,
                    m31, m32, m33, m34,
                    m41, m42, m43, m44;
        };
    } u;
};

#endif /* defined(__Matrix44_H__) */
