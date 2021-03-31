//
//  Sampler.cpp
//  raytracer
//
//  Created by Adrian Russell on 09/04/2020.
//  Copyright © 2020 Adrian Russell. All rights reserved.
//

#include "Sampler.hpp"


float RandomSampler::get1D() {
    return drand48();
}

Vector2f RandomSampler::get2D() {
    return Vector2f(drand48(), drand48());
}

Vector3f RandomSampler::get3D() {
    return Vector3f(drand48(), drand48(), drand48());
}
