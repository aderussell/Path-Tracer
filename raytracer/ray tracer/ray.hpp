//
//  Ray.hpp
//  raytracer
//
//  Created by Adrian Russell on 29/06/2018.
//  Copyright © 2018 Adrian Russell. All rights reserved.
//

#ifndef Ray_hpp
#define Ray_hpp

#include <stdio.h>
#include "Vector3.hpp"

class Ray {
    public:
    
    Ray() {}
    
    Ray(const Vector3f& a, const Vector3f& b, float ti = 0.0) {
        _a = a;
        _b = b;
        _time = ti;
    }
    
    Vector3f origin() const { return _a; }
    Vector3f direction() const { return _b; }
    float time() const { return _time; }
    Vector3f pointAtParameter(float t) const {
        return _a + (t * _b);
    }
    
    private:
    Vector3f _a;
    Vector3f _b;
    float _time;
};

#endif /* Ray_hpp */
