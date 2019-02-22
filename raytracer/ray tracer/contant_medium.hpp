//
//  contant_medium.hpp
//  raytracer
//
//  Created by Adrian Russell on 16/12/2018.
//  Copyright © 2018 Adrian Russell. All rights reserved.
//

#ifndef contant_medium_hpp
#define contant_medium_hpp

#include "hitable.hpp"
#include "texture.hpp"
#include "isotropic.hpp"

class constant_medium : public hitable  {
public:
    constant_medium(hitable *b, float d, texture *a) : boundary(b), density(d) { phase_function = new isotropic(a); }
    virtual bool hit(const Ray& r, float t_min, float t_max, SurfaceInteraction& rec) const;
    virtual bool bounding_box(float t0, float t1, aabb& box) const {
        return boundary->bounding_box(t0, t1, box); }
    hitable *boundary;
    float density;
    material *phase_function;
};

#endif /* contant_medium_hpp */
