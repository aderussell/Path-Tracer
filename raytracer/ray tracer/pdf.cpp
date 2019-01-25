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


Vector3 anisotropic_phong_pdf::generate() const {
    double xi = drand48();
    
    double phase = 0;
    bool flip = false;
    
    
    if (xi < 0.25)
    {
        xi *= 4;
    }
    else if (xi < 0.5)
    {
        xi = 1. - 4. * (0.5 - xi);
        phase = M_PI;
        flip = true;
    }
    else if (xi < 0.75)
    {
        xi = 1. - 4. * (0.75 - xi);
        phase = M_PI;
    }
    else
    {
        xi = 1. - 4. * (1. - xi);
        phase = 2. * M_PI;
        flip = true;
    }
    
    
    double phi = atan(prefactor1 * tan(M_PI_2 * xi));
    if (flip)
        phi = phase - phi;
    else
        phi += phase;
    
    const double c = cos(phi);
    const double s = sin(phi);
    const double c2 = c * c;
    const double s2 = 1. - c2;
    
    xi = drand48();
    
    phase = 0;
    flip = false;
    
    if (xi < 0.25)
    {
        xi *= 4;
    }
    else if (xi < 0.5)
    {
        xi = 1. - 4. * (0.5 - xi);
        phase = M_PI;
        flip = true;
    }
    else if (xi < 0.75)
    {
        xi = 1. - 4. * (0.75 - xi);
        phase = M_PI;
    }
    else
    {
        xi = 1. - 4. * (1. - xi);
        phase = 2. * M_PI;
        flip = true;
    }
    
    
    double theta = acos(pow(1. - xi, 1. / (nu * c2 + nv * s2 + 1.)));
    if (flip)
        theta = phase - theta;
    else
        theta += phase;
    
    const double st = sin(theta);
    const double ct = cos(theta);
    
    const double cos2 = ct * ct;
    const double sin2 = st * st;
    
    const Vector3 h = uvw.local(Vector3(st * c, st * s, ct));
    
    const double kh = Vector3::dotProduct(incident*-1, h);
    const double specularProbability = GetSpecularPDH(h, kh, cos2, sin2);
    const double weight = 1.0 + specularProbability;
    
    const double diffuseProbability = 1.0 / weight;
    
    
    if (drand48() < diffuseProbability)
    {
        //info->atten = info->diffuseColor;
        return uvw.local(random_cosine_direction());
    }
    
    //info->atten = info->specularColor;
    return GetSpecularReflected(incident, h, kh);
}
