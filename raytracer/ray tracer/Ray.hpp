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

class ray {
    public:
    
    ray() {}
    
    ray(const Vector3& a, const Vector3& b) {
        _a = a;
        _b = b;
    }
    
    Vector3 origin() const { return _a; }
    Vector3 direction() const { return _b; }
    
    Vector3 parameterAtPoint(float t) const {
        return _a + (t * _b);
    }
    
    private:
    Vector3 _a;
    Vector3 _b;
};

#endif /* Ray_hpp */
