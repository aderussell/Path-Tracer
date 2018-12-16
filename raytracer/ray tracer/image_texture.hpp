//
//  image_texture.hpp
//  raytracer
//
//  Created by Adrian Russell on 10/12/2018.
//  Copyright © 2018 Adrian Russell. All rights reserved.
//

#ifndef image_texture_hpp
#define image_texture_hpp

#include "texture.hpp"

class image_texture : public texture {
public:
    image_texture() {}
    image_texture(unsigned char *pixels, int A, int B) : data(pixels), nx(A), ny(B) {}
    
    virtual Color value(float u, float v, const Vector3& p) const;
    
    unsigned char *data;
    int nx, ny;
};

#endif /* image_texture_hpp */
