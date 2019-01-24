//
//  image_texture.cpp
//  raytracer
//
//  Created by Adrian Russell on 10/12/2018.
//  Copyright © 2018 Adrian Russell. All rights reserved.
//

#include "image_texture.hpp"

Color image_texture::value(float u, float v, const Vector3& p) const {
    int i = u*nx;
    int j = (1-v)*ny-0.001;
    if (i<0)i=0;
    if (j<0)j=0;
    if (i>nx-1) i=nx-1;
    if (j>ny-1) j=ny-1;
    int dataOffset = 3*i + 3*nx*j;
    float r = int(data[dataOffset]) / 255.0;
    float g = int(data[dataOffset+1]) / 255.0;
    float b = int(data[dataOffset+2]) / 255.0;
    //std::cout << data[dataOffset] << ", " << data[dataOffset+1] << ", " << data[dataOffset+2] << std::endl;
    return Color(r,g,b);
}

