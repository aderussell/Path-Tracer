//
//  hitable_list.cpp
//  raytracer
//
//  Created by Adrian Russell on 06/12/2018.
//  Copyright © 2018 Adrian Russell. All rights reserved.
//

#include "hitable_list.hpp"
#include <stdlib.h>

bool hitable_list::hit(const Ray &r, float t_min, float t_max, SurfaceInteraction &rec) const {
    SurfaceInteraction temp_rec;
    bool hit_anything = false;
    double closest_so_far = t_max;
    for (int i = 0; i < list_size; ++i) {
        aabb box;
        if (list[i]->bounding_box(t_min, t_max, box)) {
            if (box.hit(r, t_min, t_max)) {
                if (list[i]->hit(r, t_min, closest_so_far, temp_rec)) {
                    hit_anything = true;
                    closest_so_far = temp_rec.t;
                    rec = temp_rec;
                }
            }
        }
        
    }
    return hit_anything;
}

bool hitable_list::bounding_box(float t0, float t1, aabb &box) const {
    if (list_size < 1) { return false; }
    aabb temp_box;
    bool first_true = list[0]->bounding_box(t0, t1, temp_box);
    if (!first_true) {
        return false;
    } else {
        box = temp_box;
        for (int i = 1; i < list_size; ++i) {
            if (list[i]->bounding_box(t0, t1, temp_box)) {
                box = surrounding_box(box, temp_box);
            } else {
                return false;
            }
        }
    }
    return true;
}

float hitable_list::pdf_value(const Vector3f &o, const Vector3f &v) const {
    float weight = 1.0/list_size;
    float sum = 0;
    for (int i = 0; i < list_size; ++i) {
        sum += weight*list[i]->pdf_value(o, v);
    }
    return sum;
}

Vector3f hitable_list::random(const Vector3f &o) const {
    int index = int(drand48() * list_size);
    return list[index]->random(o);
}
