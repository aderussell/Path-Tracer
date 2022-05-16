//
//  integrator.hpp
//  raytracer
//
//  Created by Adrian Russell on 27/01/2019.
//  Copyright © 2019 Adrian Russell. All rights reserved.
//

#ifndef integrator_hpp
#define integrator_hpp

#include "scene.hpp"
#include "ImageBuffer.hpp"

class Integrator {
public:
    virtual void preprocess(const Scene &scene) { }
    virtual void render(const Scene &scene) = 0;
};



class TestIntegrator : public Integrator {
public:
    TestIntegrator(ImageBuffer *buffer, int ns = 100) : imageBuffer(buffer), ns(ns) {}
    void render(const Scene &scene);
    
private:
    ImageBuffer* imageBuffer;
    int ns;
    Color color(const Ray &r, hitable *world, hitable *light_shape, SkyBox *sky_box, int depth);
};

class TestIntegrator2 : public Integrator {
public:
    TestIntegrator2(ImageBuffer *buffer, int ns = 100) : imageBuffer(buffer), ns(ns) {
        totalsImageBuffer = new ImageBuffer(buffer->width, buffer->height);
        ns_pix.assign(buffer->width * buffer->height, 0);
    }
    void render(const Scene &scene);
    
private:
    ImageBuffer* imageBuffer;
    ImageBuffer* totalsImageBuffer;
    std::vector<size_t> ns_pix;
    int ns;
    Color color(const Ray &r, hitable *world, hitable *light_shape, SkyBox *sky_box, int depth);
};


class BasicIntegrator : public Integrator {
public:
    BasicIntegrator(ImageBuffer *buffer, int ns = 100) : imageBuffer(buffer), ns(ns) {}
    void render(const Scene &scene);
    
private:
    ImageBuffer* imageBuffer;
    int ns;
    Color color(const Ray &r, hitable *world, hitable *light_shape, SkyBox *sky_box, int depth);
};


class JitterIntegrator : public Integrator {
public:
    JitterIntegrator(ImageBuffer *buffer, int ns = 100) : imageBuffer(buffer), ns(ceil(sqrt(ns))) {}
    void render(const Scene &scene);
    
private:
    ImageBuffer* imageBuffer;
    int ns;
    Color color(const Ray &r, hitable *world, hitable *light_shape, SkyBox *sky_box, int depth);
};


class DepthIntegrator: public Integrator {
public:
    DepthIntegrator(ImageBuffer *buffer, float min, float max) : imageBuffer(buffer), min(min), max(max), depth(max - min) {}
    void render(const Scene &scene);
    float dist(const Ray &r, hitable *world, float min, float max);
private:
    ImageBuffer* imageBuffer;
    float min, max, depth;
};

class NormalIntegrator: public Integrator {
public:
    NormalIntegrator(ImageBuffer *buffer) : imageBuffer(buffer) {}
    void render(const Scene &scene);
private:
    ImageBuffer* imageBuffer;
    Color color(const Ray &r, hitable *world, hitable *light_shape, SkyBox *sky_box, int depth);
};

class AlbedoIntegrator : public Integrator {
public:
    AlbedoIntegrator(ImageBuffer *buffer) : imageBuffer(buffer) {}
    void render(const Scene &scene);
    
private:
    ImageBuffer* imageBuffer;
    Color color(const Ray &r, hitable *world, hitable *light_shape, SkyBox *sky_box, int depth);
};

#endif /* integrator_hpp */
