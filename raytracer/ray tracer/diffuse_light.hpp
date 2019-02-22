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
    
//    virtual bool scatter(const ray& ray_in, const hit_record& rec, Vector3f& attenuation, ray& scattered) const {
//        return false;
//    };
    
    virtual Color emitted(const Ray& ray_in, const hit_record& rec, float u, float v, const Vector3f& p) const {
        if (Vector3f::dotProduct(rec.normal, ray_in.direction()) < 0.0) {
            return emit->value(u, v, p);
        }
        return Color(0,0,0);
    }
    
private:
    texture *emit;
};

#endif /* diffuse_light_hpp */
