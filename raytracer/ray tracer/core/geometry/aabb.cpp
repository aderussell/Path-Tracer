//
//  aabb.cpp
//  raytracer
//
//  Created by Adrian Russell on 09/12/2018.
//  Copyright © 2018 Adrian Russell. All rights reserved.
//

#include "aabb.hpp"

aabb surrounding_box(aabb box0, aabb box1) {
    __m128 s = _mm_min_ps(box0.min()._a, box1.min()._a);
    __m128 b = _mm_max_ps(box0.max()._a, box1.max()._a);
    return aabb(s, b);
}
