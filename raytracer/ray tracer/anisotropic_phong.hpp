//
//  anisotropic_phong.hpp
//  raytracer
//
//  Created by Adrian Russell on 24/01/2019.
//  Copyright © 2019 Adrian Russell. All rights reserved.
//

#ifndef anisotropic_phong_hpp
#define anisotropic_phong_hpp

#include "Material.hpp"
#include "texture.hpp"

// https://www.cs.utah.edu/~shirley/papers/jgtbrdf.pdf


class anisotropic_phong : public material {
public:
    anisotropic_phong(texture *a, texture *s) : albedo(a), specular(s) {}
    
    bool scatter(const ray& r_in, const hit_record& rec, scatter_record& srec) const;
    float scattering_pdf(const ray& ray_in, const hit_record& rec, ray& scattered) const;
    
    
private:
    texture *albedo;
    texture *specular;
    double nu, nv;
    
    Vector3 random_in_unit_sphere() const;
};

#endif /* anisotropic_phong_hpp */
