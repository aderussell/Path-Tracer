//
//  Fresnel.hpp
//  raytracer
//
//  Created by Adrian Russell on 04/04/2020.
//  Copyright © 2020 Adrian Russell. All rights reserved.
//

#ifndef Fresnel_hpp
#define Fresnel_hpp

#include <math.h>
#include "Vector3.hpp"

namespace Fresnel
{
    Vector3f Schlick(Vector3f r0, float radians);
    float Schlick(float r0, float radians);
    float SchlickWeight(float u);
    float SchlickDielectic(float cosThetaI, float relativeIor);
    float Dielectric(float cosThetaI, float relativeIorI, float relativeIorT);
    float SchlickR0FromRelativeIOR(float eta);
}

#endif /* Fresnel_hpp */
