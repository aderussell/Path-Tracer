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
#include "skybox.h"

class scene {
public:
    scene(hitable *_world, hitable *_light_shape, camera *_camera, skybox *_sky_box, int _ratio = 1) : world(_world), light_shape(_light_shape), camera(_camera), sky_box(_sky_box), ratio(_ratio) {}

    hitable *world;
    hitable *light_shape;
    camera *camera;
    skybox *sky_box;
    float ratio;
};

#endif /* scene_h */
