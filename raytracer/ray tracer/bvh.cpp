//
//  bvh.cpp
//  raytracer
//
//  Created by Adrian Russell on 09/12/2018.
//  Copyright © 2018 Adrian Russell. All rights reserved.
//

#include "bvh.hpp"
#include <iostream>
#include <float.h>
#include <stdlib.h>

int box_x_compare(const void* a, const void* b) {
    aabb box_left, box_right;
    hitable *ah = *(hitable**)a;
    hitable *bh = *(hitable**)b;
    if (!ah->bounding_box(0, 0, box_left) || !bh->bounding_box(0, 0, box_right)) {
        std::cerr << "no bounding box in bvh_node constructor\n";
    }
    if (box_left.min().x - box_right.min().x < 0.0) {
        return -1;
    } else {
        return 1;
    }
}

int box_y_compare(const void* a, const void* b) {
    aabb box_left, box_right;
    hitable *ah = *(hitable**)a;
    hitable *bh = *(hitable**)b;
    if (!ah->bounding_box(0, 0, box_left) || !bh->bounding_box(0, 0, box_right)) {
        std::cerr << "no bounding box in bvh_node constructor\n";
    }
    if (box_left.min().y - box_right.min().y < 0.0) {
        return -1;
    } else {
        return 1;
    }
}

int box_z_compare(const void* a, const void* b) {
    aabb box_left, box_right;
    hitable *ah = *(hitable**)a;
    hitable *bh = *(hitable**)b;
    if (!ah->bounding_box(0, 0, box_left) || !bh->bounding_box(0, 0, box_right)) {
        std::cerr << "no bounding box in bvh_node constructor\n";
    }
    if (box_left.min().z - box_right.min().z < 0.0) {
        return -1;
    } else {
        return 1;
    }
}

//bvh_node::bvh_node(hitable **l, int n, float time0, float time1) {
//    int axis = int(3*drand48());
//    if (axis == 0) {
//        qsort(l, n, sizeof(hitable *), box_x_compare);
//    } else if (axis == 1) {
//        qsort(l, n, sizeof(hitable *), box_y_compare);
//    } else {
//        qsort(l, n, sizeof(hitable *), box_z_compare);
//    }
//    if (n == 1) {
//        left = right = l[0];
//    } else if (n == 2) {
//        left = l[0];
//        right = l[1];
//    } else {
//        left = new bvh_node(l, n/2, time0, time1);
//        right = new bvh_node(l + n / 2, n - n/2, time0, time1);
//    }
//
//    aabb box_left, box_right;
//    if (!left->bounding_box(time0, time1, box_left) || !right->bounding_box(time0, time1, box_right)) {
//        // throw error
//    }
//    box = surronding_box(box_left, box_right);
//}

bvh_node::bvh_node(hitable **l, int n, float time0, float time1) {
    aabb *boxes = new aabb[n];
    float *left_area = new float[n];
    float *right_area = new float[n];

    aabb main_box;

    l[0]->bounding_box(time0, time1, main_box);
    for (int i = 1; i < n; i++) {
        aabb new_box;
        l[i]->bounding_box(time0, time1, new_box);
        main_box = surronding_box(new_box, main_box);
    }

    int axis = main_box.longest_axis();
    if (axis == 0) {
        qsort(l, n, sizeof(hitable *), box_x_compare);
    } else if (axis == 1) {
        qsort(l, n, sizeof(hitable *), box_y_compare);
    } else {
        qsort(l, n, sizeof(hitable *), box_z_compare);
    }

    for (int i = 0; i < n; i++) {
        l[i]->bounding_box(time0, time1, boxes[i]);
    }
    left_area[0] = boxes[0].area();
    aabb left_box = boxes[0];
    for (int i = 1; i < n-1; i++) {
        left_box = surronding_box(left_box, boxes[i]);
        left_area[i] = left_box.area();
    }
    right_area[n-1] = boxes[n-1].area();
    aabb right_box = boxes[n-1];
    for (int i = n-2; i > 0; i--) {
        right_box = surronding_box(right_box, boxes[i]);
        right_area[i] = right_box.area();
    }
    float min_SAH = FLT_MAX;
    int min_SAH_idx = 0;
    for (int i = 0; i < n-1; i++) {
        float SAH = i*left_area[i] + (n-i-1)*right_area[i+1];
        if (SAH < min_SAH) {
            min_SAH_idx = i;
            min_SAH = SAH;
        }
    }
    
    if (min_SAH_idx == 0) {
        left = l[0];
    } else {
        left = new bvh_node(l, min_SAH_idx+1, time0, time1);
    }
    if (min_SAH_idx == n-2) {
        right = l[min_SAH_idx+1];
    } else {
        right = new bvh_node(l + min_SAH_idx+1, n - min_SAH_idx-1, time0, time1);
    }
    box = main_box;
}

bool bvh_node::bounding_box(float t0, float t1, aabb &b) const {
    b = box;
    return true;
}

bool bvh_node::hit(const ray&r, float t_min, float t_max, hit_record& rec) const {
    if (box.hit(r, t_min, t_max)) {
        hit_record left_rec, right_rec;
        bool hit_left = left->hit(r, t_min, t_max, left_rec);
        bool hit_right = right->hit(r, t_min, t_max, right_rec);
        if (hit_left && hit_right) {
            if (left_rec.t < right_rec.t) {
                rec = left_rec;
            } else {
                rec = right_rec;
            }
            return true;
        } else if (hit_left) {
            rec = left_rec;
            return true;
        } else if (hit_right) {
            rec = right_rec;
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}


float bvh_node::pdf_value(const Vector3& o, const Vector3& v) const
{
    float weight = 0.5;
    float sum = weight * left->pdf_value(o, v) + weight * right->pdf_value(o, v);
    return sum;
}

Vector3 bvh_node::random(const Vector3& o) const
{
    if (drand48() < 0.5)
        return left->random(o);
    else
        return right->random(o);
}
