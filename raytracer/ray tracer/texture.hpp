//
//  texture.hpp
//  raytracer
//
//  Created by Adrian Russell on 09/12/2018.
//  Copyright © 2018 Adrian Russell. All rights reserved.
//

#ifndef texture_hpp
#define texture_hpp

#include "Vector3.hpp"
#include "Material.hpp"
#include <math.h>

class texture {
public:
    virtual Color value(float u, float v, const Vector3f& p) const = 0;
};

class constant_texture: public texture {
public:
    constant_texture() {}
    constant_texture(Color c) : color(c) {}
    Color value(float u, float v, const Vector3f& p) const {
        return color;
    }
    
private:
    Color color;
};

class checker_texture : public texture {
public:
    checker_texture() {}
    checker_texture(texture *t0, texture *t1): even(t0), odd(t1) {}
    Color value(float u, float v, const Vector3f& p) const {
        float sines = sin(p.x / factor)*sin(p.y / factor)*sin(p.z / factor);
        if (sines < 0) {
            return odd->value(u,v,p);
        } else {
            return even->value(u,v,p);
        }
    }
private:
    texture *odd;
    texture *even;
    float factor = 15.0;
};

#endif /* texture_hpp */
