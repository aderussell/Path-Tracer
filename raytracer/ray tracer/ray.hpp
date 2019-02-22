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
    
    Ray(const Vector3f& o, const Vector3f& d, double ti = 0.0, double tMax = std::numeric_limits<double>::infinity()) : _origin(o), _direction(d), _time(ti), _tMax(tMax) {}
    
    Vector3f origin() const { return _origin; }
    Vector3f direction() const { return _direction; }
    double time() const { return _time; }
    double tMax() const { return _tMax; }
    
    
    Vector3f pointAtParameter(float t) const {
        return _origin + (t * _direction);
    }
    
    Vector3f _origin;
    Vector3f _direction;
    double _time;
    mutable double _tMax;
};

#endif /* Ray_hpp */
