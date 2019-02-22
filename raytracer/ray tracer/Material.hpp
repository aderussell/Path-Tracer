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
#include "pdf.hpp"

struct Color {
    double r, g, b;
    Color(double r = 0.0, double g = 0.0, double b = 0.0, double luminosity = 1.0) {
        this->r = r * luminosity;
        this->g = g * luminosity;
        this->b = b * luminosity;
    }
    
//    Color(const Vector3f& input) {
//        r = input.x;
//        g = input.y;
//        b = input.z;
//    }
    
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

inline Color operator / (const Color& aColor, const float b) {
    return Color(aColor.r / b, aColor.g / b, aColor.b / b);
}

inline Color operator * (double scalar, const Color &color) {
    return Color(scalar * color.r, scalar * color.g, scalar * color.b);
}

inline Color operator + (const Color& a, const Color& b) {
    return Color(a.r + b.r, a.g + b.g, a.b + b.b);
}

inline Color operator * (const Color& a, const Vector3f& b) {
    return Color(a.r * b.x, a.g * b.y, a.b * b.z);
}




struct scatter_record {
    Ray specular_ray;
    bool is_specular;
    Color attenuation;
    Color specular;
    pdf *pdf_ptr = nullptr;
};



class material {
public:
    virtual bool scatter(const Ray& ray_in, const hit_record& rec, scatter_record& srec) const {
        return false;
    };
    
    virtual float scattering_pdf(const Ray& ray_in, const hit_record& rec, Ray& scattered) const {
        return false;
    };
    
    
    virtual Color emitted(const Ray& ray_in, const hit_record& rec, float u, float v, const Vector3f& p) const {
        return Color(0,0,0);
    }
};

#endif /* Material_hpp */
