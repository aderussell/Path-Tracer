//
//  perlin.cpp
//  raytracer
//
//  Created by Adrian Russell on 10/12/2018.
//  Copyright © 2018 Adrian Russell. All rights reserved.
//

#include "perlin.hpp"

static Vector3 *perlin_generate() {
    Vector3 *p = new Vector3[256];
    for (int i = 0; i < 256; i++) {
        p[i] = Vector3(-1 + 2*drand48(), -1 + 2*drand48(), -1 + 2*drand48()).normalise();
    }
    return p;
}

void permute(int *p, int n) {
    for (int i = n-1; i > 0; i--) {
        int target = int(drand48()*(i+1));
        int tmp = p[i];
        p[i] = p[target];
        p[target] = tmp;
    }
}

static int* perlin_generate_perm() {
    int *p = new int[256];
    for (int i = 0; i < 256; i++) {
        p[i] = i;
    }
    permute(p, 256);
    return p;
}

float perlin::turb(const Vector3& p, int depth) const {
    float accum = 0;
    Vector3 temp_p = p;
    float weight = 1.0;
    for (int i = 0; i < depth; i++) {
        accum += weight * noise(temp_p);
        weight *= 0.5;
        temp_p *= 2;
    }
    return fabs(accum);
}

Vector3 *perlin::ranvec = perlin_generate();
int *perlin::perm_x = perlin_generate_perm();
int *perlin::perm_y = perlin_generate_perm();
int *perlin::perm_z = perlin_generate_perm();
