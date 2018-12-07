//
//  metal.hpp
//  raytracer
//
//  Created by Adrian Russell on 07/12/2018.
//  Copyright © 2018 Adrian Russell. All rights reserved.
//

#ifndef metal_hpp
#define metal_hpp

#include "Material.hpp"

class metal: public material {
    
public:
    metal(const Vector3& a, float f) : albedo(a) { if (f < 1) fuzz = f; else fuzz = 1; }
    
    virtual bool scatter(const ray& ray_in, const hit_record& rec, Vector3& attenuation, ray& scattered) const;
    
    Vector3 albedo;
    float fuzz; // how 'fuzzy' the reflections are
    
private:
    Vector3 random_in_unit_sphere() const;
    Vector3 reflect(const Vector3& v, const Vector3& n) const;
};

#endif /* metal_hpp */
