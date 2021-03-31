//
//  Fresnel.cpp
//  raytracer
//
//  Created by Adrian Russell on 04/04/2020.
//  Copyright © 2020 Adrian Russell. All rights reserved.
//

#include "Fresnel.hpp"
#include "aabb.hpp"

inline float Saturate(float a) {
    return ffclamp<float>(a, 0.0, 1.0);
}

inline float Square(float a) {
    return a * a;
}

inline float AbsCosTheta(Vector3f a) {
    return fabs(Square(a.y()));
}

template <typename T>
inline T Lerp(T a, T b, float t) {
    return (1.0f - t) * a + t * b;
}

namespace Fresnel
{
    //=========================================================================================================================
    Vector3f Schlick(Vector3f r0, float radians)
    {
        float exponential = powf(1.0f - radians, 5.0f);
        return r0 + (Vector3f(1.0f) - r0) * exponential;
    }

    //=========================================================================================================================
    float Schlick(float r0, float radians)
    {
        return Lerp(1.0f, Fresnel::SchlickWeight(radians), r0);
    }

    //=========================================================================================================================
    float SchlickWeight(float u)
    {
        float m = Saturate(1.0f - u);
        float m2 = m * m;
        return m * m2 * m2;
    }

    //=========================================================================================================================
    float SchlickDielectic(float cosThetaI, float relativeIor)
    {
        float r0 = SchlickR0FromRelativeIOR(relativeIor);
        return r0 + (1.0f - r0) * SchlickWeight(cosThetaI);
    }

    //=========================================================================================================================
    float Dielectric(float cosThetaI, float relativeIorI, float relativeIorT)
    {
        // Copied from PBRT. This function calculates the full Fresnel term for a dielectric material.

        cosThetaI = ffclamp(cosThetaI, -1.0f, 1.0f);

        // Swap index of refraction if this is coming from inside the surface
        if(cosThetaI < 0.0f) {
            float temp = relativeIorI;
            relativeIorI = relativeIorT;
            relativeIorT = temp;

            cosThetaI = -cosThetaI;
        }

        float sinThetaI = sqrtf(ffmax(0.0f, 1.0f - cosThetaI * cosThetaI));
        float sinThetaT = relativeIorI / relativeIorT * sinThetaI;

        // Check for total internal reflection
        if(sinThetaT >= 1) {
            return 1;
        }

        float cosThetaT = sqrtf(ffmax(0.0f, 1.0f - sinThetaT * sinThetaT));

        float Rparl = ((relativeIorT * cosThetaI) - (relativeIorI * cosThetaT))
                    / ((relativeIorT * cosThetaI) + (relativeIorI * cosThetaT));
        float Rperp = ((relativeIorI * cosThetaI) - (relativeIorT * cosThetaT))
                    / ((relativeIorI * cosThetaI) + (relativeIorT * cosThetaT));
        return (Rparl * Rparl + Rperp * Rperp) / 2;
    }

    //=========================================================================================================================
    float SchlickR0FromRelativeIOR(float eta)
    {
        // https://seblagarde.wordpress.com/2013/04/29/memo-on-fresnel-equations/
        return Square(eta - 1.0f) / Square(eta + 1.0f);
    }
}
