//
//  pbr_material.hpp
//  raytracer
//
//  Created by Adrian Russell on 08/06/2020.
//  Copyright © 2020 Adrian Russell. All rights reserved.
//

#ifndef pbr_material_hpp
#define pbr_material_hpp

#include "Material.hpp"

class pbr_material: public material {
    
public:
    pbr_material(const Color& a, Color s, float pS, float r) : albedo(a), specularColor(s), percentSpecular(pS), roughness(r) { }
    
    virtual bool scatter(const Ray& ray_in, const SurfaceInteraction& hrec, scatter_record& srec) const;
    virtual float scattering_pdf(const Ray& ray_in, const SurfaceInteraction& rec, Ray& scattered) const;
    
private:
    Color albedo;           // the color used for diffuse lighting
    float percentSpecular; // percentage chance of doing specular instead of diffuse lighting
    float roughness;       // how rough the specular reflections are
    Color specularColor;    // the color tint of specular reflections
    
    Vector3f random_in_unit_sphere() const;
    Vector3f reflect(const Vector3f& v, const Vector3f& n) const;
};

#endif /* pbr_material_hpp */
