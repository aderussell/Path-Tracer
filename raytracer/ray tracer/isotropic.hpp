//
//  isotropic.hpp
//  raytracer
//
//  Created by Adrian Russell on 16/12/2018.
//  Copyright © 2018 Adrian Russell. All rights reserved.
//

#ifndef isotropic_hpp
#define isotropic_hpp

#include "Material.hpp"
#include "texture.hpp"

class isotropic : public material {
public:
    isotropic(texture *a) : albedo(a) {}
    virtual bool scatter(const ray& r_in, const hit_record& rec, scatter_record& srec) const;
    
private:
    texture *albedo;
    Vector3f random_in_unit_sphere() const;
};

#endif /* isotropic_hpp */
