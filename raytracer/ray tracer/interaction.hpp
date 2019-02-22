//
//  interaction.hpp
//  raytracer
//
//  Created by Adrian Russell on 22/02/2019.
//  Copyright © 2019 Adrian Russell. All rights reserved.
//

#ifndef interaction_hpp
#define interaction_hpp

#include "Vector3.hpp"
class material;

class Interaction {
    
};

class SurfaceInteraction : public Interaction {
public:
    double t;            // time
    Vector3f p;          // position
    Vector3f normal;
    material *mat_ptr;
    double u, v;
};

#endif /* interaction_hpp */
