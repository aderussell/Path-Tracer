//
//  RayTracer.hpp
//  raytracer
//
//  Created by Adrian Russell on 29/06/2018.
//  Copyright © 2018 Adrian Russell. All rights reserved.
//

#ifndef RayTracer_hpp
#define RayTracer_hpp

#include <stdio.h>
#include "ImageBuffer.hpp"

class RayTracer {

public:
    int width;
    int height;
    int ns = 10000;
    
    int bounceDepth;
    int shadowBias;
    
    RayTracer(int width, int height) : width(width), height(height) {}
    
    ImageBuffer* render();
};

#endif /* RayTracer_hpp */
