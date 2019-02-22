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
#include "skybox.hpp"
#include <memory>
#include <vector>

// temporary declaration of light until it is used
class Light {};

class Scene {
public:
    
    Scene(std::shared_ptr<hitable> world, std::shared_ptr<SkyBox> skybox, const std::vector<std::shared_ptr<Light>> &lights) : world(world), sky_box(skybox), lights(lights) {}
    
    
    Scene(hitable *_world, hitable *_light_shape, camera *_camera, SkyBox *_sky_box, int _ratio = 1) : world(_world), light_shape(_light_shape), camera(_camera), sky_box(_sky_box) {}

    std::shared_ptr<hitable> world;
    std::vector<std::shared_ptr<Light>> lights;
    
    hitable *light_shape;
    camera *camera;
    
    std::shared_ptr<SkyBox> sky_box;
};

#endif /* scene_h */
