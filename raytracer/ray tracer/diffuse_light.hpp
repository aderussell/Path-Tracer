//
//  diffuse_light.hpp
//  raytracer
//
//  Created by Adrian Russell on 13/12/2018.
//  Copyright © 2018 Adrian Russell. All rights reserved.
//

#ifndef diffuse_light_hpp
#define diffuse_light_hpp

#include "Material.hpp"
#include "texture.hpp"

class diffuse_light : public material {
public:
    diffuse_light(texture *a) : emit(a) {}
    
    virtual bool scatter(const ray& ray_in, const hit_record& rec, Vector3& attenuation, ray& scattered) const {
        return false;
    };
    
    virtual Color emitted(float u, float v, const Vector3& p) const {
        return emit->value(u,v,p);
    }
    
private:
    texture *emit;
};

#endif /* diffuse_light_hpp */
