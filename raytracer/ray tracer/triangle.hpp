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
#include "Material.hpp"

class triangle : public hitable {
public:
    triangle(const Vector3f& _a, const Vector3f& _b, const Vector3f& _c, material *m) : a(_a), b(_b), c(_c), mat(m) {}
    
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
    bool bounding_box(float t0, float t1, aabb &box) const;
    virtual float pdf_value(const Vector3f& o, const Vector3f& v) const;
    virtual Vector3f random(const Vector3f& o) const;
    
protected:
    Vector3f a, b, c;
    material *mat;
};

class triangle_with_normals : public triangle {
public:
    triangle_with_normals(const Vector3f& _a, const Vector3f& _b, const Vector3f& _c, material *m, const Vector3f& n1, const Vector3f& n2, const Vector3f& n3) : triangle(_a, _b, _c, m), na(n1), nb(n2), nc(n3) {}
    
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
    
private:
    Vector3f na, nb, nc;
};

#endif /* triangle_hpp */
