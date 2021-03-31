//
//  hittable_sets.cpp
//  raytracer
//
//  Created by Adrian Russell on 24/05/2020.
//  Copyright © 2020 Adrian Russell. All rights reserved.
//

#include "hittable_sets.hpp"
#include <stdlib.h>

bool hitable_union::hit(const Ray&r, float t_min, float t_max, SurfaceInteraction& rec) const {
    SurfaceInteraction temp_rec;
    bool hit_anything = false;
    double closest_so_far = t_max;
    hitable* list[2] = { left, right };
    for (int i = 0; i < 2; ++i) {
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

bool hitable_union::bounding_box(float t0, float t1, aabb &box) const {
    aabb l, r;
    left->bounding_box(t0, t1, l);
    right->bounding_box(t0, t1, r);
    box = surrounding_box(l, r);
    return true;
}

float hitable_union::pdf_value(const Vector3f& o, const Vector3f& v) const {
    float sum = 0.5*left->pdf_value(o, v);
    sum += 0.5*right->pdf_value(o, v);
    return sum;
}

Vector3f hitable_union::random(const Vector3f& o) const {
    hitable * item = drand48() >= 0.5 ? right : left;
    return item->random(o);
}


bool hitable_difference::hit(const Ray&r, float t_min, float t_max, SurfaceInteraction& rec) const {
    SurfaceInteraction left_rec;
    SurfaceInteraction right_rec;
    double closest_so_far = t_max;
    bool left_hit = false;
    bool right_hit = false;
    aabb box;
    if (left->bounding_box(t_min, t_max, box)) {
        left_hit = left->hit(r, t_min, t_max, left_rec);
        if (left_hit) {
            closest_so_far = left_rec.t;
        }
    }
    if (right->bounding_box(t_min, t_max, box)) {
        right_hit = right->hit(r, t_min, t_max, right_rec);
        if (right_hit) {
            closest_so_far = left_rec.t;
        }
    }
    if (left_hit && right_hit) {
        return false;
    }
    if (left_hit && closest_so_far == left_rec.t) {
        rec = left_rec;
        return true;
    }
    if (right_hit && closest_so_far == right_rec.t) {
        rec = right_rec;
        return true;
    }
    return false;
}
bool hitable_difference::bounding_box(float t0, float t1, aabb &box) const {
    aabb l, r;
    left->bounding_box(t0, t1, l);
    right->bounding_box(t0, t1, r);
    box = surrounding_box(l, r);
    return true;
}
float hitable_difference::pdf_value(const Vector3f& o, const Vector3f& v) const {
    float sum = 0.5*left->pdf_value(o, v);
    sum += 0.5*right->pdf_value(o, v);
    return sum;
}
Vector3f hitable_difference::random(const Vector3f& o) const {
    hitable * item = drand48() >= 0.5 ? right : left;
    return item->random(o);
}


bool hitable_intersection::hit(const Ray&r, float t_min, float t_max, SurfaceInteraction& rec) const {
    SurfaceInteraction left_rec;
    SurfaceInteraction right_rec;
    double closest_so_far = t_max;
    bool left_hit = false;
    bool right_hit = false;
    aabb box;
    if (left->bounding_box(t_min, t_max, box)) {
        left_hit = left->hit(r, t_min, t_max, left_rec);
        if (left_hit) {
            closest_so_far = left_rec.t;
        }
    }
    if (right->bounding_box(t_min, t_max, box)) {
        right_hit = right->hit(r, t_min, t_max, right_rec);
        if (right_hit) {
            closest_so_far = left_rec.t;
        }
    }
    if (!(left_hit && right_hit)) {
        return false;
    }
    if (left_hit && closest_so_far == left_rec.t) {
        rec = left_rec;
        return true;
    }
    if (right_hit && closest_so_far == right_rec.t) {
        rec = right_rec;
        return true;
    }
    return false;
}
bool hitable_intersection::bounding_box(float t0, float t1, aabb &box) const {
    aabb l, r;
    left->bounding_box(t0, t1, l);
    right->bounding_box(t0, t1, r);
    box = overlapping_box(l, r);
    return true;
}
float hitable_intersection::pdf_value(const Vector3f& o, const Vector3f& v) const {
    float sum = 0.5*left->pdf_value(o, v);
    sum += 0.5*right->pdf_value(o, v);
    return sum;
}
Vector3f hitable_intersection::random(const Vector3f& o) const {
    hitable * item = drand48() >= 0.5 ? right : left;
    return item->random(o);
}
