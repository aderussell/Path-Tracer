//
//  onb.cpp
//  raytracer
//
//  Created by Adrian Russell on 18/12/2018.
//  Copyright © 2018 Adrian Russell. All rights reserved.
//

#include "onb.hpp"

void onb::build_from_w(const Vector3& n) {
    axis[2] = n.normalise();
    Vector3 a;
    if (fabs(w().x) > 0.9) {
        a = Vector3(0,1,0);
    } else {
        a = Vector3(1,0,0);
    }
    axis[1] = Vector3::crossProduct(w(), a).normalise();
    axis[0] = Vector3::crossProduct(w(), v());
}
