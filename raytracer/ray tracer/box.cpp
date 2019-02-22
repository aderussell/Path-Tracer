//
//  box.cpp
//  raytracer
//
//  Created by Adrian Russell on 16/12/2018.
//  Copyright © 2018 Adrian Russell. All rights reserved.
//

#include "box.hpp"

box::box(const Vector3f& p0, const Vector3f& p1, material *ptr) {
    pmin = p0;
    pmax = p1;
    hitable **list = new hitable*[6];
    list[0] = new xy_rect(p0.x, p1.x, p0.y, p1.y, p1.z, ptr);
    list[1] = new flip_normals(new xy_rect(p0.x, p1.x, p0.y, p1.y, p0.z, ptr));
    list[2] = new xz_rect(p0.x, p1.x, p0.z, p1.z, p1.y, ptr);
    list[3] = new flip_normals(new xz_rect(p0.x, p1.x, p0.z, p1.z, p0.y, ptr));
    list[4] = new yz_rect(p0.y, p1.y, p0.z, p1.z, p1.x, ptr);
    list[5] = new flip_normals(new yz_rect(p0.y, p1.y, p0.z, p1.z, p0.x, ptr));
    list_ptr = new hitable_list(list,6);
}

bool box::hit(const Ray& r, float t0, float t1, SurfaceInteraction& rec) const {
    return list_ptr->hit(r, t0, t1, rec);
}
