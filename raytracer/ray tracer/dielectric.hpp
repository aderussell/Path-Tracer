//
//  dielectric.hpp
//  raytracer
//
//  Created by Adrian Russell on 07/12/2018.
//  Copyright © 2018 Adrian Russell. All rights reserved.
//

#ifndef dielectric_hpp
#define dielectric_hpp

#include "Material.hpp"

class dielectric: public material {
    
public:
    dielectric(float ri) : ref_idx(ri) { }
    
    virtual bool scatter(const ray& ray_in, const hit_record& rec, Vector3& attenuation, ray& scattered) const;
    
    float ref_idx;
    
private:
    bool refract(const Vector3& v, const Vector3& n, float ni_over_nt, Vector3& refracted) const;
    Vector3 reflect(const Vector3& v, const Vector3& n) const;
    float schlick(float cosine, float ref_idx) const;
};

#endif /* dielectric_hpp */
