//
//  integrator.hpp
//  raytracer
//
//  Created by Adrian Russell on 27/01/2019.
//  Copyright © 2019 Adrian Russell. All rights reserved.
//

#ifndef integrator_hpp
#define integrator_hpp

#include "scene.h"
#include "ImageBuffer.hpp"

class Integrator {
public:
    virtual void preprocess(const scene &scene) { }
    virtual void render(const scene &scene) = 0;
};



class TestIntegrator : public Integrator {
public:
    TestIntegrator(ImageBuffer *buffer, int ns = 100) : imageBuffer(buffer), ns(ns) {}
    void render(const scene &scene);
    
private:
    ImageBuffer* imageBuffer;
    int ns;
    Color color(const ray &r, hitable *world, hitable *light_shape, skybox *sky_box, int depth);
};


class BasicIntegrator : public Integrator {
public:
    BasicIntegrator(ImageBuffer *buffer, int ns = 100) : imageBuffer(buffer), ns(ns) {}
    void render(const scene &scene);
    
private:
    ImageBuffer* imageBuffer;
    int ns;
    Color color(const ray &r, hitable *world, hitable *light_shape, skybox *sky_box, int depth);
};


class JitterIntegrator : public Integrator {
public:
    JitterIntegrator(ImageBuffer *buffer, int ns = 100) : imageBuffer(buffer), ns(ceil(sqrt(ns))) {}
    void render(const scene &scene);
    
private:
    ImageBuffer* imageBuffer;
    int ns;
    Color color(const ray &r, hitable *world, hitable *light_shape, skybox *sky_box, int depth);
};


#endif /* integrator_hpp */
