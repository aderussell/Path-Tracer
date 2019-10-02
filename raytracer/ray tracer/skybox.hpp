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
#include "ray.hpp"
#include "Material.hpp"
#include "image_texture.hpp"

class SkyBox {
public:
    virtual Color get_color(const Ray& ray) const = 0;
};

class constant_skybox : public SkyBox {
public:
    constant_skybox(Color c = Color(0,0,0)) : color(c) {}
    
    virtual Color get_color(const Ray& ray) const {
        return color;
    }
    
private:
    Color color;
};

class sky_skybox : public SkyBox {
public:
    virtual Color get_color(const Ray& r) const {
        Vector3f unit_direction = r.direction().normalized();
        float t = 0.5*(unit_direction.y() + 1.0);
        return (1.0-t)*Color(1,1,1) + t*Color(0.5,0.7,1.0);
    }
};

class cubemap_skybox : public SkyBox {
public:
    cubemap_skybox(image_texture* top, image_texture* bottom, image_texture* left, image_texture* right, image_texture* front, image_texture* back) : _up(top), _down(bottom), _left(left), _right(right), _front(front), _back(back) {}
    
    virtual Color get_color(const Ray& r) const {
        Vector3f unit_direction = r.direction();
        
        if (fabs(unit_direction.x()) >= fabs(unit_direction.y()) && fabs(unit_direction.x()) >= fabs(unit_direction.z())) {
            if (unit_direction.x() >= 0.0) {
                double u = 1.0 - (unit_direction.z() / unit_direction.x() + 1.0) / 2;
                double v = 1.0 - (unit_direction.y() / unit_direction.x() + 1.0) / 2;
                return _right->value(u,v, unit_direction);
            } else {
                double u = (unit_direction.z() / unit_direction.x() + 1.0) / 2;
                double v = (unit_direction.y() / unit_direction.x() + 1.0) / 2;
                return _left->value(u,v, unit_direction);
            }
        } else if (fabs(unit_direction.y()) >= fabs(unit_direction.x()) && fabs(unit_direction.y()) >= fabs(unit_direction.z())) {
            if (unit_direction.y() >= 0.0) {
                double u = (unit_direction.x() / unit_direction.y() + 1.0) / 2;
                double v = (unit_direction.z() / unit_direction.y() + 1.0) / 2;
                return _up->value(u,v, unit_direction);
            } else {
                double u = 1.0 - (unit_direction.x() / unit_direction.y() + 1.0) / 2;
                double v = 1.0 - (unit_direction.z() / unit_direction.y() + 1.0) / 2;
                return _down->value(u,v, unit_direction);
            }
        } else {
            if (unit_direction.z() >= 0.0) {
                double u = (unit_direction.x() / unit_direction.z() + 1.0) / 2;
                double v = 1.0 - (unit_direction.y() / unit_direction.z() + 1.0) / 2;
                return _front->value(u,v, unit_direction);
            } else {
                double u = (unit_direction.x() / unit_direction.z() + 1.0) / 2;
                double v = (unit_direction.y() / unit_direction.z() + 1.0) / 2;
                return _back->value(u,v, unit_direction);
            }
        }
    }
    
private:
    image_texture *_up, *_down, *_left, *_right, *_front, *_back;
};

#endif /* skybox_h */
