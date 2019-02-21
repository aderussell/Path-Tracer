//
//  Matrix44.cpp
//  Drawing
//
//  Created by Adrian Russell on 10/8/13.
//  Copyright (c) 2013 Adrian Russell. All rights reserved.
//

#include <cmath>
#include "Matrix44.hpp"
//#include <cstring>

#define USE_FAST_CALCULATIONS 0

Matrix44::Matrix44(double m11, double m12, double m13, double m14,
         double m21, double m22, double m23, double m24,
         double m31, double m32, double m33, double m34,
         double m41, double m42, double m43, double m44)
{
    this->u.m11 = m11; this->u.m12 = m12; this->u.m13 = m13; this->u.m14 = m14;
    this->u.m21 = m21; this->u.m22 = m22; this->u.m23 = m23; this->u.m24 = m24;
    this->u.m31 = m31; this->u.m32 = m32; this->u.m33 = m33; this->u.m34 = m34;
    this->u.m41 = m41; this->u.m42 = m42; this->u.m43 = m43; this->u.m44 = m44;
}

Matrix44::Matrix44(double v[4][4])
{
    for (unsigned int i = 0; i < 4; i++) {
        for (unsigned int j = 0; j < 4; j++) {
            this->u.v[i][j] = v[i][j];
        }
    }
}

Matrix44::Matrix44(double a[16])
{
    for (unsigned int i = 0; i < 16; i++) {
        this->u.a[i] = a[i];
    }
    //memcpy(this->u.a, a, sizeof(double) * 16);
    //std::copy(&a[0], &a[15], this->u.a);
}

Matrix44 Matrix44::emptyMatrix()
{
    return Matrix44();
}


Matrix44 Matrix44::indentityMatrix()
{
    return Matrix44(1, 0, 0, 0,
                    0, 1, 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1);
}

Matrix44 Matrix44::translate(double tx, double ty, double tz)
{
    return Matrix44(1,  0,  0,  0,
                    0,  1,  0,  0,
                    0,  0,  1,  0,
                    tx, ty, tz, 1);
}

Matrix44 Matrix44::scale(double sx, double sy, double sz)
{
    return Matrix44(sx, 0,  0,  0,
                    0,  sy, 0,  0,
                    0,  0,  sz, 0,
                    0,  0,  0,  1);
}

Matrix44 Matrix44::rotationX(double a)
{
    return Matrix44(1, 0,       0,      0,
                    0, cos(a),  sin(a), 0,
                    0, -sin(a), cos(a), 0,
                    0, 0,       0,      1);
}

Matrix44 Matrix44::rotationY(double a)
{
    return Matrix44(cos(a), 0, -sin(a), 0,
                    0,      1, 0,       0,
                    sin(a), 0, cos(a),  0,
                    0,      0, 0,       1);
}


Matrix44 Matrix44::rotationZ(double a)
{
    return Matrix44(cos(a),  sin(a), 0, 0,
                    -sin(a), cos(a), 0, 0,
                    0,       0,      1, 0,
                    0,       0,      0, 1);
}

Matrix44 Matrix44::rotationYawPitchRoll(double y, double p, double r)
{
    Matrix44 yaw   = rotationZ(-y);
    Matrix44 pitch = rotationY(-p);
    Matrix44 roll  = rotationX(-r);
    return yaw * pitch * roll;
}

Matrix44 Matrix44::perspective(double f, double n)
{
    return Matrix44(1, 0, 0,                  0,
                    0, 1, 0,                  0,
                    0, 0, 1,  -1,
                    0, 0, -((f*n) / (f - n)), 0);
}

Matrix44 Matrix44::perspective(double f)
{
    return Matrix44(1, 0, 0, 0,
                    0, 1, 0, 0,
                    0, 0, 1, (1.0 / f),
                    0, 0, 0, 0);
}



Matrix44 Matrix44::Orthographic(double left, double right, double bottom, double top, double nearZ, double farZ)
{
    double mtx[16];
    
    //See appendix G of OpenGL Red Book
    
    mtx[ 0] = 2.0f / (right - left);
    mtx[ 1] = 0.0;
    mtx[ 2] = 0.0;
    mtx[ 3] = 0.0;
    
    mtx[ 4] = 0.0;
    mtx[ 5] = 2.0f / (top - bottom);
    mtx[ 6] = 0.0;
    mtx[ 7] = 0.0;
    
    mtx[ 8] = 0.0;
    mtx[ 9] = 0.0;
    mtx[10] = -2.0f / (farZ - nearZ);
    mtx[11] = 0.0;
    
    mtx[12] = -(right + left) / (right - left);
    mtx[13] = -(top + bottom) / (top - bottom);
    mtx[14] = -(farZ + nearZ) / (farZ - nearZ);
    mtx[15] = 1.0f;
    
    return Matrix44(mtx);
}

//Matrix44 Matrix44::mtxLoadPerspective(double fov, double aspect, double nearZ, double farZ)
//{
//    float f = 1.0f / tanf( (fov * (M_PI/180)) / 2.0f);
//    
//    double mtx[16];
//    
//    mtx[0] = f / aspect;
//    mtx[1] = 0.0f;
//    mtx[2] = 0.0f;
//    mtx[3] = 0.0f;
//    
//    mtx[4] = 0.0f;
//    mtx[5] = f;
//    mtx[6] = 0.0f;
//    mtx[7] = 0.0f;
//    
//    mtx[8] = 0.0f;
//    mtx[9] = 0.0f;
//    mtx[10] = (farZ+nearZ) / (nearZ-farZ);
//    mtx[11] = -1.0f;
//    
//    mtx[12] = 0.0f;
//    mtx[13] = 0.0f;
//    mtx[14] = 2 * farZ * nearZ /  (nearZ-farZ);
//    mtx[15] = 0.0f;
//    
//    return Matrix44(mtx);
//}

Matrix44 Matrix44::mtxLoadPerspective(double fov, double aspect, double nearZ, double farZ)
{
    //float f = 1.0f / tanf( (fov * (M_PI/180)) / 2.0f);
    float h = (1 / tan(fov/2.0));
    float w = h / aspect;
    
    double mtx[16];
    
    mtx[0] = w;
    mtx[1] = 0.0f;
    mtx[2] = 0.0f;
    mtx[3] = 0.0f;
    
    mtx[4] = 0.0f;
    mtx[5] = h;
    mtx[6] = 0.0f;
    mtx[7] = 0.0f;
    
    mtx[8] = 0.0f;
    mtx[9] = 0.0f;
    mtx[10] = (farZ) / (farZ - nearZ);
    mtx[11] = 1.0f;
    
    mtx[12] = 0.0f;
    mtx[13] = 0.0f;
    mtx[14] = -farZ * nearZ / (farZ - nearZ);
    mtx[15] = 0.0f;
    
    return Matrix44(mtx);
}

Matrix44 Matrix44::lookAtLH(Vector3f position, Vector3f target, Vector3f upVector)
{
    Vector3f zAxis = (target - position).normalized();
    Vector3f xAxis = Vector3f::crossProduct(upVector, zAxis).normalized();
    Vector3f yAxis = Vector3f::crossProduct(zAxis, xAxis);
    
    return Matrix44(xAxis.x, yAxis.x, zAxis.x, 0,
                    xAxis.y, yAxis.y, zAxis.y, 0,
                    xAxis.z, yAxis.z, zAxis.z, 0,
                    -Vector3f::dotProduct(xAxis, position), -Vector3f::dotProduct(yAxis, position), -Vector3f::dotProduct(yAxis, position), 1);
}


bool Matrix44::operator== (Matrix44 &m)
{
#if USE_FAST_CALCULATIONS
    if (this->u.m11 != m.u.m11) return false;
    if (this->u.m12 != m.u.m12) return false;
    if (this->u.m13 != m.u.m13) return false;
    if (this->u.m14 != m.u.m14) return false;
    if (this->u.m21 != m.u.m21) return false;
    if (this->u.m22 != m.u.m22) return false;
    if (this->u.m23 != m.u.m23) return false;
    if (this->u.m24 != m.u.m24) return false;
    if (this->u.m31 != m.u.m31) return false;
    if (this->u.m32 != m.u.m32) return false;
    if (this->u.m33 != m.u.m33) return false;
    if (this->u.m34 != m.u.m34) return false;
    if (this->u.m41 != m.u.m41) return false;
    if (this->u.m42 != m.u.m42) return false;
    if (this->u.m43 != m.u.m43) return false;
    if (this->u.m44 != m.u.m44) return false;
    return true;
#else
    for (unsigned int i = 0; i < 16; i++) {
            if (this->u.a[i] != m.u.a[i]) {
                return false;
            }
    }
    return true;
#endif
}

bool Matrix44::operator!= (Matrix44 &m)
{
    return !(*this == m);
}


bool Matrix44::isEmpty()
{
    Matrix44 empty = Matrix44::emptyMatrix();
    return (*this != empty);
}

Matrix44 Matrix44::operator+ (Matrix44 m)
{
#if USE_FAST_CALCULATIONS
    return Matrix44(this->u.m11 + m.u.m11, this->u.m12 + m.u.m12, this->u.m13 + m.u.m13, this->u.m14 + m.u.m14,
                    this->u.m21 + m.u.m21, this->u.m22 + m.u.m22, this->u.m23 + m.u.m23, this->u.m24 + m.u.m24,
                    this->u.m31 + m.u.m31, this->u.m32 + m.u.m32, this->u.m33 + m.u.m33, this->u.m34 + m.u.m34,
                    this->u.m41 + m.u.m41, this->u.m42 + m.u.m42, this->u.m43 + m.u.m43, this->u.m44 + m.u.m44);
#else
    Matrix44 result;
    for (unsigned int i = 0; i < 4; i++) {
        for (unsigned int j = 0; j < 4; j++) {
            result.u.v[i][j] = this->u.v[i][j] + m.u.v[i][j];
        }
    }
    return result;
#endif
}

Matrix44 Matrix44::operator- (Matrix44 m)
{
#if USE_FAST_CALCULATIONS
    return Matrix44(this->u.m11 - m.u.m11, this->u.m12 - m.u.m12, this->u.m13 - m.u.m13, this->u.m14 - m.u.m14,
                    this->u.m21 - m.u.m21, this->u.m22 - m.u.m22, this->u.m23 - m.u.m23, this->u.m24 - m.u.m24,
                    this->u.m31 - m.u.m31, this->u.m32 - m.u.m32, this->u.m33 - m.u.m33, this->u.m34 - m.u.m34,
                    this->u.m41 - m.u.m41, this->u.m42 - m.u.m42, this->u.m43 - m.u.m43, this->u.m44 - m.u.m44);
#else
    Matrix44 result;
    for (unsigned int i = 0; i < 4; i++) {
        for (unsigned int j = 0; j < 4; j++) {
            result.u.v[i][j] = this->u.v[i][j] - m.u.v[i][j];
        }
    }
    return result;
#endif
}


Matrix44 Matrix44::operator* (Matrix44 m)
{
#if USE_FAST_CALCULATIONS
    double nm11 = (this->u.m11 * m.u.m11) + (this->u.m12 * m.u.m21) + (this->u.m13 * m.u.m31) + (this->u.m14 * m.u.m41);
    double nm12 = (this->u.m11 * m.u.m12) + (this->u.m12 * m.u.m22) + (this->u.m13 * m.u.m32) + (this->u.m14 * m.u.m42);
    double nm13 = (this->u.m11 * m.u.m13) + (this->u.m12 * m.u.m23) + (this->u.m13 * m.u.m33) + (this->u.m14 * m.u.m43);
    double nm14 = (this->u.m11 * m.u.m14) + (this->u.m12 * m.u.m24) + (this->u.m13 * m.u.m34) + (this->u.m14 * m.u.m44);
    double nm21 = 0;
    double nm22 = 0;
    double nm23 = 0;
    double nm24 = 0;
    double nm31 = 0;
    double nm32 = 0;
    double nm33 = 0;
    double nm34 = 0;
    double nm41 = 0;
    double nm42 = 0;
    double nm43 = 0;
    double nm44 = 0;
    
    return Matrix44(nm11, nm12, nm13, nm14,
                    nm21, nm22, nm23, nm24,
                    nm31, nm32, nm33, nm34,
                    nm41, nm42, nm43, nm44);
#else
    Matrix44 result;
    for (unsigned int i = 0; i < 4; i++) {
        for (unsigned int j = 0; j < 4; j++) {
            for (unsigned int k = 0; k < 4; k++) {
                result.u.v[i][j] += this->u.v[i][k] * m.u.v[k][j];
            }
        }
    }
    return result;
#endif
}

Matrix44 Matrix44::operator* (double s)
{
#if USE_FAST_CALCULATIONS
    return Matrix44(this->u.m11 * s, this->u.m12 * s, this->u.m13 * s, this->u.m14 * s,
                    this->u.m21 * s, this->u.m22 * s, this->u.m23 * s, this->u.m24 * s,
                    this->u.m31 * s, this->u.m32 * s, this->u.m33 * s, this->u.m34 * s,
                    this->u.m41 * s, this->u.m42 * s, this->u.m43 * s, this->u.m44 * s);
#else
    Matrix44 result;
    for (unsigned int i = 0; i < 4; i++) {
        for (unsigned int j = 0; j < 4; j++) {
            result.u.v[i][j] = this->u.v[i][j] * s;
        }
    }
    return result;
#endif
}


Matrix44 Matrix44::transpose()
{
    return Matrix44(this->u.m11, this->u.m21, this->u.m31, this->u.m41,
                    this->u.m12, this->u.m22, this->u.m32, this->u.m42,
                    this->u.m13, this->u.m23, this->u.m33, this->u.m43,
                    this->u.m14, this->u.m24, this->u.m34, this->u.m44);
}

Matrix44 Matrix44::inverse()
{
    //calculate inverse
    return Matrix44();
}



