//
//  scene.h
//  raytracer
//
//  Created by Adrian Russell on 21/12/2018.
//  Copyright © 2018 Adrian Russell. All rights reserved.
//

#ifndef scene_h
#define scene_h

#include "camera.hpp"
#include "hitable.hpp"

class scene {
public:
    scene(hitable *_world, hitable *_light_shape, camera *_camera, int _ratio = 1) : world(_world), light_shape(_light_shape), camera(_camera), ratio(_ratio) {}

    hitable *world;
    hitable *light_shape;
    camera *camera;
    float ratio;
};

#endif /* scene_h */
