//
//  testscene.hpp
//  raytracer
//
//  Created by Adrian Russell on 22/02/2019.
//  Copyright © 2019 Adrian Russell. All rights reserved.
//

#ifndef testscene_hpp
#define testscene_hpp

#include "scene.hpp"
#include "camera.hpp"

class TestScene {
public:
    
    TestScene(std::unique_ptr<Scene> scene, std::unique_ptr<Camera> camera) : scene(std::move(scene)), camera(std::move(camera)) {}
    
    std::unique_ptr<Scene> scene;
    std::unique_ptr<Camera> camera;
};

#endif /* testscene_hpp */
