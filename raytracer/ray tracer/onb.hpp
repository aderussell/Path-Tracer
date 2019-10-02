//
//  onb.hpp
//  raytracer
//
//  Created by Adrian Russell on 18/12/2018.
//  Copyright © 2018 Adrian Russell. All rights reserved.
//

#ifndef onb_hpp
#define onb_hpp

#include "Vector3.hpp"

//ortho-normal base
class onb {
public:
    onb() {}
    onb(const Vector3f& n) { build_from_w(n); } // normal
    onb(const Vector3f& n, const Vector3f& i) { build(n,i); } //normal & incident ray vector
    
    inline Vector3f operator[](int i) const { return axis[i]; }
    Vector3f u() const { return axis[0]; }
    Vector3f v() const { return axis[1]; }
    Vector3f w() const { return axis[2]; }
    Vector3f local(float a, float b, float c) const { return a*u() + b*v() + c*w(); }
    Vector3f local(const Vector3f& a) const { return local(a.x(), a.y(), a.z()); }
    void build_from_w(const Vector3f& n);
    void build(const Vector3f& n, const Vector3f& i);
private:
    Vector3f axis[3];
};

#endif /* onb_hpp */
