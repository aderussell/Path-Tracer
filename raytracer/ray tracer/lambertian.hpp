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
#include "texture.hpp"

class lambertian: public material {
    
public:
    lambertian(texture* a) : albedo(a) {}
    
    virtual bool scatter(const ray& ray_in, const hit_record& rec, Color& albedo, ray& scattered, float& pdf) const;
    virtual float scattering_pdf(const ray& ray_in, const hit_record& rec, ray& scattered) const;
    
    texture *albedo;
};

#endif /* lambertian_hpp */
