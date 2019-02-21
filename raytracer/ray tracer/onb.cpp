//
//  onb.cpp
//  raytracer
//
//  Created by Adrian Russell on 18/12/2018.
//  Copyright © 2018 Adrian Russell. All rights reserved.
//

#include "onb.hpp"

void onb::build_from_w(const Vector3f& n) {
    axis[2] = n.normalized();
    Vector3f a;
    if (fabs(w().x) > 0.9) {
        a = Vector3f(0,1,0);
    } else {
        a = Vector3f(1,0,0);
    }
    axis[1] = Vector3f::crossProduct(w(), a).normalized();
    axis[0] = Vector3f::crossProduct(w(), v());
}

void onb::build(const Vector3f& n, const Vector3f& i) {
    double cosine = abs(Vector3f::dotProduct(n, i));
    if (cosine >= 1) {
        build_from_w(n);
    } else {
        axis[2] = n.normalized();
        axis[0] = Vector3f::crossProduct(i, n).normalized();
        axis[1] = Vector3f::crossProduct(w(), u());
    }    
}
