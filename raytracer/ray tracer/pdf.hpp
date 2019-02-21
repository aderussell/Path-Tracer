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
    virtual ~pdf() {};
    virtual float value(const Vector3f& direction) const = 0;
    virtual Vector3f generate() const = 0;
};



class cosine_pdf : public pdf {
public:
    cosine_pdf(const Vector3f& w) { uvw.build_from_w(w); }
    
    virtual float value(const Vector3f& direction) const;
    virtual Vector3f generate() const;
    
private:
    onb uvw;
};



class hitable_pdf : public pdf {
public:
    hitable_pdf(hitable *p, const Vector3f& origin) : ptr(p), o(origin) {}
    
    virtual float value(const Vector3f& direction) const {
        return ptr->pdf_value(o, direction);
    };
    virtual Vector3f generate() const {
        return ptr->random(o);
    };
    
private:
    Vector3f o;
    hitable *ptr;
};



class mixture_pdf : public pdf {
public:
    mixture_pdf(pdf *p0, pdf *p1) { p[0] = p0; p[1] = p1; }
    
    float value(const Vector3f& direction) const {
        return 0.5 * p[0]->value(direction) + 0.5 * p[1]->value(direction);
    };
    Vector3f generate() const {
        if (drand48() < 0.5) {
            return p[0]->generate();
        } else {
            return p[1]->generate();
        }
    };
    
private:
    pdf *p[2];
};




class anisotropic_phong_pdf : public pdf {
public:
    anisotropic_phong_pdf(const Vector3f& inc, const Vector3f& norm, double Nu, double Nv) : incident(inc), uvw(norm, inc), nu(Nu), nv(Nv) {
        prefactor1 = sqrt((nu + 1.0) / (nv + 1.0));
        prefactor2 = sqrt((nu + 1.0) * (nv + 1.0)) / (2.0 * M_PI);
    }
    
    float value(const Vector3f& direction) const {
        double cosine = Vector3f::dotProduct(direction, uvw.w());
        if (cosine < 0) {
            return 0;
        }
        
        return cosine / M_PI;
    };
    Vector3f generate() const;
    
private:
    
    inline Vector3f GetSpecularReflected(const Vector3f& incident, const Vector3f& h, double kh) const
    {
        return incident + 2.0 * kh * h;
    }
    
    inline double GetSpecularPDH(const Vector3f& h, double kh, double cos2, double sin2) const
    {
        return GetHPDH(h, cos2, sin2) / (4.0 * kh);
    }
    
    inline double GetHPDH(const Vector3f& h, double cos2, double sin2) const
    {
        const double nh = Vector3f::dotProduct(h, uvw.w());
        
        return prefactor2 * pow(nh, nu * cos2 + nv * sin2);
    }
    
    
    Vector3f incident;
    onb uvw;
    double nu;
    double nv;
    double prefactor1, prefactor2;
};


#endif /* pdf_hpp */
