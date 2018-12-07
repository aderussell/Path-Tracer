//
//  hitable_list.hpp
//  raytracer
//
//  Created by Adrian Russell on 06/12/2018.
//  Copyright © 2018 Adrian Russell. All rights reserved.
//

#ifndef hitable_list_hpp
#define hitable_list_hpp

#include "hitable.hpp"

class hitable_list: public hitable {
public:
    hitable_list() {}
    hitable_list(hitable **l, int n) { list = l; list_size = n; }
    hitable **list;
    int list_size;
    
    virtual bool hit(const ray&r, float t_min, float t_max, hit_record& rec) const;
};

#endif /* hitable_list_hpp */
