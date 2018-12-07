//
//  Vector4.cpp
//  Drawing
//
//  Created by Adrian Russell on 20/01/2014.
//  Copyright (c) 2014 Adrian Russell. All rights reserved.
//

#include "Vector4.h"

const Vector4 Vector4::Vector4Zero = Vector4(0.0, 0.0, 0.0, 0.0);

const Vector4 Vector4::Vector4NoPoint = Vector4(INFINITY, INFINITY, INFINITY, INFINITY);

Vector4::Vector4(double x, double y, double z, double w)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

Vector4::Vector4(Vector3 v)
{
    this->x = v.x;
    this->y = v.y;
    this->z = v.z;
    this->w = 1;
}

Vector4 Vector4::operator+= (Vector4 v)
{
    this->x += v.x;
    this->y += v.y;
    this->z += v.z;
    this->w += v.w;
    return *this;
}


Vector4 Vector4::operator-= (Vector4 v)
{
    this->x -= v.x;
    this->y -= v.y;
    this->z -= v.z;
    this->w -= v.w;
    return *this;
}


Vector4 Vector4::operator*= (double s)
{
    this->x *= s;
    this->y *= s;
    this->z *= s;
    this->w *= s;
    return *this;
}

Vector4 Vector4::operator/= (double s)
{
    this->x /= s;
    this->y /= s;
    this->z /= s;
    this->w /= s;
    return *this;
}


Vector4 Vector4::operator+ (Vector4 v)
{
    return Vector4(this->x + v.x, this->y + v.y, this->z + v.z, this->w + v.w);
}

Vector4 Vector4::operator- (Vector4 v)
{
    return Vector4(this->x - v.x, this->y - v.y, this->z - v.z, this->w - v.w);
}

Vector4 Vector4::operator* (double s)
{
    return Vector4(*this) *= s;
}

Vector4 Vector4::operator/ (double s)
{
    return Vector4(*this) /= s;
}

Vector4 Vector4::operator- ()
{
    return Vector4(*this) * -1.0;
}

bool Vector4::operator== (Vector4 &v)
{
    return ((x == v.x) && (y == v.y) && (z == v.z) && (w == v.w));
}

bool Vector4::operator!= (Vector4 &v)
{
    return !((x == v.x) && (y == v.y) && (z == v.z) && (w == v.w));
}

Vector4 Vector4::inverse()
{
    return Vector4(-this->x, -this->y, -this->z, -this->w);
}

Vector4 Vector4::normaliseScale()
{
    if (w == 0.0) return Vector4();
    return Vector4(x/w, y/w, z/w, 1.0);
}

Vector4 Vector4::transform(const Matrix44 &m) const
{
    double x = (this->x * m.u.m11) + (this->y * m.u.m21) + (this->z * m.u.m31) + (this->w * m.u.m41);
    double y = (this->x * m.u.m12) + (this->y * m.u.m22) + (this->z * m.u.m32) + (this->w * m.u.m42);
    double z = (this->x * m.u.m13) + (this->y * m.u.m23) + (this->z * m.u.m33) + (this->w * m.u.m43);
    double w = (this->x * m.u.m14) + (this->y * m.u.m24) + (this->z * m.u.m34) + (this->w * m.u.m44);
    return Vector4(x, y, z, w);
}


