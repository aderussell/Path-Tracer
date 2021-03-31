//
//  Sampler.hpp
//  raytracer
//
//  Created by Adrian Russell on 09/04/2020.
//  Copyright © 2020 Adrian Russell. All rights reserved.
//

#ifndef Sampler_hpp
#define Sampler_hpp

#include <stdio.h>
#include "Vector2.hpp"
#include "Vector3.hpp"

class Sampler {
public:
    virtual float get1D() = 0;
    virtual Vector2f get2D() = 0;
    virtual Vector3f get3D() = 0;
};

class RandomSampler: public Sampler {
public:
    float get1D();
    Vector2f get2D();
    Vector3f get3D();
};

#endif /* Sampler_hpp */
