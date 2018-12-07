//
//  Vector3.cpp
//  Drawing
//
//  Created by Adrian Russell on 10/8/13.
//  Copyright (c) 2013 Adrian Russell. All rights reserved.
//

#include "Vector3.h"


Vector3::Vector3(double x, double y, double z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

Vector3& Vector3::operator+= (const Vector3& v)
{
    this->x += v.x;
    this->y += v.y;
    this->z += v.z;
    return *this;
}


Vector3& Vector3::operator-= (const Vector3 &v)
{
    this->x -= v.x;
    this->y -= v.y;
    this->z -= v.z;
    return *this;
}


Vector3& Vector3::operator*= (const double s)
{
    this->x *= s;
    this->y *= s;
    this->z *= s;
    return *this;
}

Vector3& Vector3::operator/= (const double s)
{
    this->x /= s;
    this->y /= s;
    this->z /= s;
    return *this;
}


const Vector3 Vector3::operator+ (const Vector3 &v)
{
    return Vector3(*this) += v;
}

const Vector3 Vector3::operator- (const Vector3 &v)
{
    return Vector3(*this) -= v;
}

const Vector3 Vector3::operator* (const double s) const
{
    return Vector3(*this) *= s;
}

Vector3 Vector3::operator/ (double s)
{
    return Vector3(*this) /= s;
}

bool Vector3::operator== (const Vector3 &v)
{
    return ((x == v.x) && (y == v.y) && (z == v.z));
}

bool Vector3::operator!= (const Vector3 &v)
{
    return !((x == v.x) && (y == v.y) && (z == v.z));
}

Vector3 Vector3::normal() const
{
    return Vector3(-y, x, z);
}

//const Vector3 Vector3::transform(const double angle)
//{
//    double cs = cos(angle);
//    double sn = sin(angle);
//    //cs = rounddp(cs, 5);
//    //sn = rounddp(sn, 5);
//    double px = x * cs - y * sn;
//    double py = x * sn + y * cs;
//    
//    return Vector3(px, py);
//}

Vector3 Vector3::normalise() const
{
    return (this->x == 0.0f && this->y == 0.0f && this->z == 0.0f) ? Vector3(*this) : (*this * (1.0 / this->length()));
}

double Vector3::squareMagnitude() const
{
    return (this->x * this->x) + (this->y * this->y) + (this->z * this->z);
}

double Vector3::magnitude() const
{
    return sqrt(this->squareMagnitude());
}

double Vector3::length() const
{
    return this->magnitude();
}

double Vector3::dotProduct(Vector3 v1, Vector3 v2)
{
    return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}


Vector3 Vector3::crossProduct(Vector3 v1, Vector3 v2)
{
    return Vector3(v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x);
}

Vector3 Vector3::normalisedCrossProduct(Vector3 v1, Vector3 v2)
{
    return Vector3(v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x).normalise();
}

double Vector3::distance(Vector3 v1, Vector3 v2)
{
    double distx = (v2.x - v1.x);
    double disty = (v2.y - v1.y);
    double distz = (v2.z - v1.z);
    return sqrt( (distx * distx) + (disty * disty) + (distz * distz) );
}

//static const Vector3 vectorWithMagnitudeRotation(const double magnitude, const double angle)
//{
//    double x = cos(angle) * magnitude;
//    double y = sin(angle) * magnitude;
//    return Vector3(x, y);
//}
