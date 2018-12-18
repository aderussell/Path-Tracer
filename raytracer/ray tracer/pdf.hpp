//
//  pdf.hpp
//  raytracer
//
//  Created by Adrian Russell on 18/12/2018.
//  Copyright © 2018 Adrian Russell. All rights reserved.
//

#ifndef pdf_hpp
#define pdf_hpp

#include "Vector3.hpp"
#include "onb.hpp"
#include "hitable.hpp"
#include <stdlib.h>

class pdf {
public:
    virtual float value(const Vector3& direction) const = 0;
    virtual Vector3 generate() const = 0;
};

class cosine_pdf : public pdf {
public:
    cosine_pdf(const Vector3& w) { uvw.build_from_w(w); }
    
    virtual float value(const Vector3& direction) const;
    virtual Vector3 generate() const;
    
private:
    onb uvw;
};

class hitable_pdf : public pdf {
public:
    hitable_pdf(hitable *p, const Vector3& origin) : ptr(p), o(origin) {}
    
    virtual float value(const Vector3& direction) const {
        return ptr->pdf_value(o, direction);
    };
    virtual Vector3 generate() const {
        return ptr->random(o);
    };
    
private:
    Vector3 o;
    hitable *ptr;
};

class mixture_pdf : public pdf {
public:
    mixture_pdf(pdf *p0, pdf *p1) { p[0] = p0; p[1] = p1; }
    
    virtual float value(const Vector3& direction) const {
        return 0.5 * p[0]->value(direction) + 0.5 * p[1]->value(direction);
    };
    virtual Vector3 generate() const {
        if (drand48() < 0.5) {
            return p[0]->generate();
        } else {
            return p[1]->generate();
        }
    };
    
private:
    pdf *p[2];
};

#endif /* pdf_hpp */
