//
//  ImageBuffer.hpp
//  raytracer
//
//  Created by Adrian Russell on 29/06/2018.
//  Copyright © 2018 Adrian Russell. All rights reserved.
//

#ifndef ImageBuffer_hpp
#define ImageBuffer_hpp

#include <vector>

class ImageBuffer {
public:
    int width;
    int height;
    std::vector<Color> pixels;
    
    ImageBuffer(int width, int height, Color defaultColor = Color(0,0,0)) : width(width), height(height) {
        pixels.assign(width * height, defaultColor);
    }
    ~ImageBuffer() {}
    
    void setColor(size_t x, size_t y, Color c) {
        pixels[x + y*width] = c;
    }
};

#endif /* ImageBuffer_hpp */
