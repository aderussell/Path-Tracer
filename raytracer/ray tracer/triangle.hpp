//
//  triangle.hpp
//  raytracer
//
//  Created by Adrian Russell on 24/12/2018.
//  Copyright © 2018 Adrian Russell. All rights reserved.
//

#ifndef triangle_hpp
#define triangle_hpp

#include "hitable.hpp"

class triangle : public hitable {
public:
    triangle(const Vector3& _a, const Vector3& _b, const Vector3& _c, material *m) : a(_a), b(_b), c(_c), material(m) {}
    
    bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
    bool bounding_box(float t0, float t1, aabb &box) const;
    virtual float pdf_value(const Vector3& o, const Vector3& v) const;
    virtual Vector3 random(const Vector3& o) const;
    
private:
    Vector3 a, b, c;
    material *material;
};

#endif /* triangle_hpp */
