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
    inline Vector3 operator[](int i) const { return axis[i]; }
    Vector3 u() const { return axis[0]; }
    Vector3 v() const { return axis[1]; }
    Vector3 w() const { return axis[2]; }
    Vector3 local(float a, float b, float c) const { return a*u() + b*v() + c*w(); }
    Vector3 local(const Vector3& a) const { return local(a.x, a.y, a.z); }
    void build_from_w(const Vector3& n);
private:
    Vector3 axis[3];
};

#endif /* onb_hpp */
