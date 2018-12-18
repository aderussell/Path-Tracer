//
//  pdf.cpp
//  raytracer
//
//  Created by Adrian Russell on 18/12/2018.
//  Copyright © 2018 Adrian Russell. All rights reserved.
//

#include "pdf.hpp"
#include <stdlib.h>

inline Vector3 random_cosine_direction() {
    float r1 = drand48();
    float r2 = drand48();
    float z = sqrt(1-r2);
    float phi = 2*M_PI*r1;
    float x = cos(phi)*2*sqrt(r2);
    float y = sin(phi)*2*sqrt(r2);
    return Vector3(x,y,z);
}

float cosine_pdf::value(const Vector3& direction) const {
    float cosine = Vector3::dotProduct(direction.normalise(), uvw.w());
    if (cosine > 0) {
        return cosine / M_PI;
    } else {
        return 0;
    }
}

Vector3 cosine_pdf::generate() const {
    return uvw.local(random_cosine_direction());
}
