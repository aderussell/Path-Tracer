//
//  cylinder.hpp
//  raytracer
//
//  Created by Adrian Russell on 14/07/2019.
//  Copyright © 2019 Adrian Russell. All rights reserved.
//

#ifndef cylinder_hpp
#define cylinder_hpp

#include <cmath>
#include "hitable.hpp"
#include "Vector2.hpp"
#include "Quadratics.hpp"


class quadric: hitable {
    
    
    virtual double area() const = 0;
};

class quadric_sphere: quadric {
    
    
    quadric_sphere(double radius, double zMin, double zMax, double phiMax) :
        radius(radius),
        zMin(ffclamp(ffmin(zMin, zMax), -radius, radius)),
        zMax(ffclamp(ffmax(zMin, zMax), -radius, radius)),
        thetaMin(std::acos(ffclamp(zMin / radius, -1.0, 1.0))),
        thetaMax(std::acos(ffclamp(zMax / radius, -1.0, 1.0))),
        phiMax(ffclamp(phiMax, 0.0, M_PI*2.0)) {}
    
    bool bounding_box(float t0, float t1, aabb& box) const {
        box = aabb(Vector3f(-radius, -radius, zMin), Vector3f( radius,  radius, zMax));
        return true;
    };
    
//    bool Intersect(const Ray &ray, double *tHit, SurfaceInteraction *isect, bool testAlphaTexture) const {
//
//        double phi;
//        Vector3f pHit;
//
//        double ox(ray.origin().x), oy(ray.origin().y), oz(ray.origin().z);
//        double dx(ray.direction().x), dy(ray.direction().y), dz(ray.direction().z);
//
//        double a = dx * dx + dy * dy + dz * dz;
//        double b = 2 * (dx * ox + dy * oy + dz * oz);
//        double c = ox * ox + oy * oy + oz * oz - (radius * radius);
//
//        double t0, t1;
//        if (!quadratic(a, b, c, &t0, &t1))
//            return false;
//
//
//        // Check quadric shape _t0_ and _t1_ for nearest intersection
//        if (t0 > ray.tMax() || t1 <= 0) return false;
//        double tShapeHit = t0;
//        if (tShapeHit <= 0) {
//            tShapeHit = t1;
//            if (tShapeHit > ray.tMax()) return false;
//        }
//
//        // Compute sphere hit position and $\phi$
//        pHit = ray.pointAtParameter(tShapeHit);
//
//        // Refine sphere intersection point
//        pHit *= radius / Vector3f::distance(pHit, Vector3f(0, 0, 0));
//        if (pHit.x == 0 && pHit.y == 0) pHit.x = 1e-5f * radius;
//        phi = std::atan2(pHit.y, pHit.x);
//        if (phi < 0) phi += 2 * M_PI;
//
//        // Test sphere intersection against clipping parameters
//        if ((zMin > -radius && pHit.z < zMin) || (zMax < radius && pHit.z > zMax) ||
//            phi > phiMax) {
//            if (tShapeHit == t1) return false;
//            if (t1 > ray.tMax()) return false;
//            tShapeHit = t1;
//            // Compute sphere hit position and $\phi$
//            pHit = ray.pointAtParameter(tShapeHit);
//
//            // Refine sphere intersection point
//            pHit *= radius / Vector3f::distance(pHit, Vector3f(0, 0, 0));
//            if (pHit.x == 0 && pHit.y == 0) pHit.x = 1e-5f * radius;
//            phi = std::atan2(pHit.y, pHit.x);
//            if (phi < 0) phi += 2 * M_PI;
//            if ((zMin > -radius && pHit.z < zMin) ||
//                (zMax < radius && pHit.z > zMax) || phi > phiMax)
//                return false;
//        }
//
//        // Find parametric representation of sphere hit
//        double u = phi / phiMax;
//        double theta = std::acos(ffclamp(pHit.z / radius, -1.0, 1.0));
//        double v = (theta - thetaMin) / (thetaMax - thetaMin);
//
//        // Compute sphere $\dpdu$ and $\dpdv$
//        double zRadius = std::sqrt(pHit.x * pHit.x + pHit.y * pHit.y);
//        double invZRadius = 1 / zRadius;
//        double cosPhi = pHit.x * invZRadius;
//        double sinPhi = pHit.y * invZRadius;
//        Vector3f dpdu(-phiMax * pHit.y, phiMax * pHit.x, 0);
//        Vector3f dpdv =
//        (thetaMax - thetaMin) *
//        Vector3f(pHit.z * cosPhi, pHit.z * sinPhi, -radius * std::sin(theta));
//
//        // Compute sphere $\dndu$ and $\dndv$
//        Vector3f d2Pduu = -phiMax * phiMax * Vector3f(pHit.x, pHit.y, 0);
//        Vector3f d2Pduv =
//        (thetaMax - thetaMin) * pHit.z * phiMax * Vector3f(-sinPhi, cosPhi, 0.);
//        Vector3f d2Pdvv = -(thetaMax - thetaMin) * (thetaMax - thetaMin) *
//        Vector3f(pHit.x, pHit.y, pHit.z);
//
//        // Compute coefficients for fundamental forms
//        double E = Vector3f::dotProduct(dpdu, dpdu);
//        double F = Vector3f::dotProduct(dpdu, dpdv);
//        double G = Vector3f::dotProduct(dpdv, dpdv);
//        Vector3f N = Vector3f::crossProduct(dpdu, dpdv).normalized();
//        double e = Vector3f::dotProduct(N, d2Pduu);
//        double f = Vector3f::dotProduct(N, d2Pduv);
//        double g = Vector3f::dotProduct(N, d2Pdvv);
//
//        // Compute $\dndu$ and $\dndv$ from fundamental form coefficients
//        double invEGF2 = 1 / (E * G - F * F);
//        Vector3f dndu = Vector3f((f * F - e * G) * invEGF2 * dpdu +
//                                 (e * F - f * E) * invEGF2 * dpdv);
//        Vector3f dndv = Vector3f((g * F - f * G) * invEGF2 * dpdu +
//                                 (f * F - g * E) * invEGF2 * dpdv);
//
//        // Compute error bounds for sphere intersection
//        //Vector3f pError = gamma(5) * pHit.abs();
//
//        // Initialize _SurfaceInteraction_ from parametric information
//        (*ObjectToWorld)(SurfaceInteraction(pHit, pError, Vector2f(u, v),
//                                                     -ray.d, dpdu, dpdv, dndu, dndv,
//                                                     ray.time, this));
//
//
//
//        // Update _tHit_ for quadric intersection
//        *tHit = (double)tShapeHit;
//        return true;
//    }
    
    bool IntersectP(const Ray &ray, bool testAlphaTexture) const;
    
    double area() const;
    
    //Interaction Sample(const Point2f &u) const;
    
    //Interaction Sample(const Interaction &ref, const Point2f &u) const;
    
    double Pdf(const Interaction &ref, const Vector3f &wi) const;
    
    
private:
    const double radius;
    const double zMin, zMax;
    const double thetaMin, thetaMax, phiMax;
};

class cylinder: quadric {
    
};

#endif /* cylinder_hpp */
