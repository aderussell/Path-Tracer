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
    anisotropic_phong(texture *a, texture *s, double nu, double nv) : albedo(a), specular(s), nu(nu), nv(nv) {}
    
    bool scatter(const Ray& r_in, const SurfaceInteraction& rec, scatter_record& srec) const;
    float scattering_pdf(const Ray& ray_in, const SurfaceInteraction& rec, Ray& scattered) const;
    
    
private:
    texture *albedo;
    texture *specular;
    double nu, nv;
    
    Vector3f random_in_unit_sphere() const;
};

#endif /* anisotropic_phong_hpp */
