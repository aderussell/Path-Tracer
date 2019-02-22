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
    metal(const Color& a, float f) : albedo(a) { if (f < 1) fuzz = f; else fuzz = 1; }
    
    virtual bool scatter(const Ray& ray_in, const SurfaceInteraction& hrec, scatter_record& srec) const;
    
private:
    Color albedo;
    float fuzz; // how 'fuzzy' the reflections are
    
    Vector3f random_in_unit_sphere() const;
    Vector3f reflect(const Vector3f& v, const Vector3f& n) const;
};

#endif /* metal_hpp */
