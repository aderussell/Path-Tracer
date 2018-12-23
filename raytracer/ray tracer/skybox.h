//
//  skybox.h
//  raytracer
//
//  Created by Adrian Russell on 23/12/2018.
//  Copyright © 2018 Adrian Russell. All rights reserved.
//

#ifndef skybox_h
#define skybox_h

#include "Vector3.hpp"
#include "Ray.hpp"
#include "Material.hpp"

class skybox {
public:
    virtual Color get_color(const ray& ray) const = 0;
};

class constant_skybox : public skybox {
public:
    constant_skybox(Color c = Color(0,0,0)) : color(c) {}
    
    virtual Color get_color(const ray& ray) const {
        return color;
    }
    
private:
    Color color;
};

class sky_skybox : public skybox {
public:
    virtual Color get_color(const ray& r) const {
        Vector3 unit_direction = r.direction().normalise();
        float t = 0.5*(unit_direction.y + 1.0);
        return (1.0-t)*Color(1,1,1) + t*Color(0.5,0.7,1.0);
    }
};

#endif /* skybox_h */
