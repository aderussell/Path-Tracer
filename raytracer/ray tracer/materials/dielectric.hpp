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
    
    virtual bool scatter(const Ray& ray_in, const hit_record& rec, scatter_record& srec) const;
    
    float density = 0.0;
    Color volumeColor = Color(1,1,1);
    
private:
    float ref_idx;
    
    bool refract(const Vector3f& v, const Vector3f& n, float ni_over_nt, Vector3f& refracted) const;
    Vector3f reflect(const Vector3f& v, const Vector3f& n) const;
    float schlick(float cosine, float ref_idx) const;
};

#endif /* dielectric_hpp */
