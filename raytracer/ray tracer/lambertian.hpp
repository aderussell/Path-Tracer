//
//  lambertian.hpp
//  raytracer
//
//  Created by Adrian Russell on 07/12/2018.
//  Copyright © 2018 Adrian Russell. All rights reserved.
//

#ifndef lambertian_hpp
#define lambertian_hpp

#include "Material.hpp"

class lambertian: public material {
    
public:
    lambertian(const Vector3& a) : albedo(a) {}
    
    virtual bool scatter(const ray& ray_in, const hit_record& rec, Vector3& attenuation, ray& scattered) const;
    
    Vector3 albedo;
};

#endif /* lambertian_hpp */
