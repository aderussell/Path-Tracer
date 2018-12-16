//
//  Material.hpp
//  raytracer
//
//  Created by Adrian Russell on 29/06/2018.
//  Copyright © 2018 Adrian Russell. All rights reserved.
//

#ifndef Material_hpp
#define Material_hpp

#include "Vector3.hpp"
#include "hitable.hpp"

struct Color {
    double r, g, b;
    Color(double r = 0.0, double g = 0.0, double b = 0.0, double luminosity = 1.0) {
        this->r = r * luminosity;
        this->g = g * luminosity;
        this->b = b * luminosity;
    }
    
    Color& operator += (const Color& other) {
        r += other.r;
        g += other.g;
        b += other.b;
        return *this;
    }
    
    Color& operator *= (const Color& other) {
        r *= other.r;
        g *= other.g;
        b *= other.b;
        return *this;
    }
    
    Color& operator *= (double factor) {
        r *= factor;
        g *= factor;
        b *= factor;
        return *this;
    }
    
    Color& operator /= (double denom) {
        r /= denom;
        g /= denom;
        b /= denom;
        return *this;
    }
    
};

inline Color operator * (const Color& aColor, const Color& bColor) {
    return Color(aColor.r * bColor.r, aColor.g * bColor.g, aColor.b * bColor.b);
}

inline Color operator * (const Color& aColor, const float b) {
    return Color(aColor.r * b, aColor.g * b, aColor.b * b);
}

inline Color operator * (double scalar, const Color &color) {
    return Color(scalar * color.r, scalar * color.g, scalar * color.b);
}

inline Color operator + (const Color& a, const Color& b) {
    return Color(a.r + b.r, a.g + b.g, a.b + b.b);
}

inline Color operator * (const Color& a, const Vector3& b) {
    return Color(a.r * b.x, a.g * b.y, a.b * b.z);
}


class material {
public:
    virtual bool scatter(const ray& ray_in, const hit_record& rec, Vector3& attenuation, ray& scattered) const = 0;
    
    virtual Color emitted(float u, float v, const Vector3& p) const {
        return Color(0,0,0);
    }
};


class PhongMaterial {
    // ambient
    Color ambientReflectance;
    
    // diffuse
    Color diffuseReflectance;
    
    // specular
    Color specularReflectance;
    
    // mirror
    Color mirrorReflectance;
    
    double refractionIndex;
    double reflectionIndex;
    
    // shine
    double shininess;
    
    PhongMaterial(double ra, double ga, double ba, double rd, double gd, double bd, double rs, double gs, double bs, double rr, double gr, double br, double shine) {
        
        this->ambientReflectance  = Color(ra, ba, ga);
        this->diffuseReflectance  = Color(rd, bd, gd);
        this->specularReflectance = Color(rs, bs, gs);
        this->mirrorReflectance   = Color(rr, br, gr);
        this->shininess = shine;
    }
};

#endif /* Material_hpp */
