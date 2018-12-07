//
//  ImageBuffer.hpp
//  raytracer
//
//  Created by Adrian Russell on 29/06/2018.
//  Copyright © 2018 Adrian Russell. All rights reserved.
//

#ifndef ImageBuffer_hpp
#define ImageBuffer_hpp

#import <vector>
#include <memory>
#include <algorithm>
#include "Material.hpp"

class ImageBuffer {
  
public:
    int width;
    int height;
    std::vector<Color> pixels;
    
    ImageBuffer(int width, int height, Color defaultColor) : width(width), height(height) {
        pixels.assign(width * height, defaultColor);
    }
    ~ImageBuffer() {}
};

#endif /* ImageBuffer_hpp */
