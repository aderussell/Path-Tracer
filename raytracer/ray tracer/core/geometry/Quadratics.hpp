//
//  Quadratics.hpp
//  raytracer
//
//  Created by Adrian Russell on 03/08/2019.
//  Copyright © 2019 Adrian Russell. All rights reserved.
//

#ifndef Quadratics_h
#define Quadratics_h


inline bool quadratic(double a, double b, double c, double *t0, double *t1) {
    double discrim = (b * b) - 4 * a * c;
    if (discrim < 0) return false;
    double rootDiscrim = std::sqrt(discrim);
    
    double q;
    if (b < 0) q = -.5 * (b - rootDiscrim);
    else       q = -.5 * (b + rootDiscrim);
    *t0 = q / a;
    *t1 = c / q;
    if (*t0 > *t1) std::swap(*t0, *t1);
    return true;
}


#endif /* Quadratics_h */
