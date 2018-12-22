//
//  aabb.cpp
//  raytracer
//
//  Created by Adrian Russell on 09/12/2018.
//  Copyright © 2018 Adrian Russell. All rights reserved.
//

#include "aabb.hpp"

aabb surronding_box(aabb box0, aabb box1) {
    Vector3 small(fmin(box0.min().x, box1.min().x),
                  fmin(box0.min().y, box1.min().y),
                  fmin(box0.min().z, box1.min().z));
    Vector3 big(fmax(box0.max().x, box1.max().x),
                fmax(box0.max().y, box1.max().y),
                fmax(box0.max().z, box1.max().z));
    return aabb(small, big);
}