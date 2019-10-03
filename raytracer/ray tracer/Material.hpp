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
#include <x86intrin.h>

struct Color {
    
    union {
        __m128 _a;
        float _f[4];
        uint32_t _i[4];
    };
    
    float r() const {
        return _f[0];
    }

    float g() const {
        return _f[1];
    }

    float b() const {
        return _f[2];
    }

    float &r() {
        return _f[0];
    }

    float &g() {
        return _f[1];
    }

    float &b() {
        return _f[2];
    }
    
    Color(const __m128 &a) : _a(a) {}
    
    
    Color(float r = 0.0, float g = 0.0, float b = 0.0, float luminosity = 1.0) {
        r = r * luminosity;
        g = g * luminosity;
        b = b * luminosity;
        _a = _mm_set_ps(0, b, g, r);
    }
    
    Color& operator += (const Color& other) {
        _a = _mm_add_ps(_a, other._a);
        return *this;
    }
    
    Color& operator *= (const Color& other) {
        _a = _mm_mul_ps(_a, other._a);
        return *this;
    }
    
    Color& operator *= (double factor) {
        __m128 v = _mm_set1_ps(factor);
        _a = _mm_mul_ps(_a, v);
        return *this;
    }
    
    Color& operator /= (double denom) {
        __m128 v = _mm_set1_ps(denom);
        _a = _mm_div_ps(_a, v);
        return *this;
    }
    
};

inline Color operator * (const Color& a, const Color& b) {
    return _mm_mul_ps(a._a, b._a);
}

inline Color operator * (const Color& aColor, const float b) {
    __m128 v = _mm_set1_ps(b);
    return _mm_mul_ps(aColor._a, v);
}

inline Color operator / (const Color& aColor, const float b) {
    __m128 v = _mm_set1_ps(b);
    return _mm_div_ps(aColor._a, v);
}

inline Color operator * (double scalar, const Color &color) {
    __m128 v = _mm_set1_ps(scalar);
    return _mm_mul_ps(color._a, v);
}

inline Color operator + (const Color& a, const Color& b) {
    return _mm_add_ps(a._a, b._a);
}

inline Color operator * (const Color& a, const Vector3f& b) {
    return _mm_mul_ps(a._a, b._a);
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
    virtual bool scatter(const Ray& ray_in, const SurfaceInteraction& rec, scatter_record& srec) const {
        return false;
    };
    
    virtual float scattering_pdf(const Ray& ray_in, const SurfaceInteraction& rec, Ray& scattered) const {
        return false;
    };
    
    
    virtual Color emitted(const Ray& ray_in, const SurfaceInteraction& rec, float u, float v, const Vector3f& p) const {
        return Color(0,0,0);
    }
};

#endif /* Material_hpp */
