//
//  perlin.hpp
//  raytracer
//
//  Created by Adrian Russell on 10/12/2018.
//  Copyright © 2018 Adrian Russell. All rights reserved.
//

#ifndef perlin_hpp
#define perlin_hpp

#include "Vector3.hpp"
#include <stdlib.h>
#include "texture.hpp"

inline float trilinear_interp(float c[2][2][2], float u, float v, float w) {
    float accum = 0;
    for (int i=0; i < 2; i++) {
        for (int j=0; j < 2; j++) {
            for (int k=0; k < 2; k++) {
                accum += (i*u + (1-i)*(1-u))*
                (j*v + (1-j)*(1-v))*
                (k*w + (1-k)*(1-w))*c[i][j][k];
            }
        }
    }
    return accum;
}

inline float perlin_interp(Vector3f c[2][2][2], float u, float v, float w) {
    float uu = u*u*(3-2*u);
    float vv = v*v*(3-2*v);
    float ww = w*w*(3-2*w);
    float accum = 0;
    for (int i=0; i < 2; i++) {
        for (int j=0; j < 2; j++) {
            for (int k=0; k < 2; k++) {
                Vector3f weight_v(u-i, v-j, w-k);
                accum += (i*uu + (1-i)*(1-uu))*
                (j*vv + (1-j)*(1-vv))*
                (k*ww + (1-k)*(1-ww))* Vector3f::dotProduct(c[i][j][k], weight_v);
            }
        }
    }
    return accum;
}

class perlin {
public:
    float noise(const Vector3f& p) const {
        float u = p.x() - floor(p.x());
        float v = p.y() - floor(p.y());
        float w = p.z() - floor(p.z());
//        u = u*u*(3-2*u);
//        v = v*v*(3-2*v);
//        w = w*w*(3-2*w);
        int i = floor(p.x());
        int j = floor(p.y());
        int k = floor(p.z());
        Vector3f c[2][2][2];
        for (int di = 0; di < 2; di++) {
            for (int dj = 0; dj < 2; dj++) {
                for (int dk = 0; dk < 2; dk++) {
                    c[di][dj][dk] = ranvec[perm_x[(i+di) & 255] ^ perm_y[(j+dj) & 255] ^ perm_z[(k+dk) & 255]];
                }
            }
        }
        return perlin_interp(c, u, v, w);
    }
    
    float turb(const Vector3f& p, int depth=7) const;
    
    static Vector3f *ranvec;
    static int *perm_x;
    static int *perm_y;
    static int *perm_z;
};





class noise_texture : public texture {
public:
    noise_texture() {}
    noise_texture(float sc) : scale(sc) {}
    Color value(float u, float v, const Vector3f& p) const {
        return Color(1,1,1)*0.5*(1 + sin(scale * p.z() + 10*noise.turb(p)));
    }
    perlin noise;
    float scale;
};

#endif /* perlin_hpp */
