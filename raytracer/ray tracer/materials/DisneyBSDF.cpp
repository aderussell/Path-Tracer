//
//  DisneyBSDF.cpp
//  raytracer
//
//  Created by Adrian Russell on 04/04/2020.
//  Copyright © 2020 Adrian Russell. All rights reserved.
//

#include "DisneyBSDF.hpp"
#include "onb.hpp"
#include "Fresnel.hpp"
#include <math.h>
#include <stdlib.h>

//inline float Saturate(float a) {
//    return ffclamp<float>(a, 0.0, 1.0);
//}
//
//inline float Square(float a) {
//    return a * a;
//}
//
//inline float AbsCosTheta(Vector3f a) {
//    return fabs(a.y());
//}
//
//inline float CosTheta(Vector3f a) {
//    return a.y();
//}
//
//inline float Cos2Theta(Vector3f w)
//{
//    return w.y() * w.y();
//}
//
//inline float Sin2Theta(Vector3f w) {
//    return ffmax(0.0f, 1.0f - Cos2Theta(w));
//}
//
//inline float SinTheta(Vector3f w) {
//    return sqrtf(Sin2Theta(w));
//}
//
//inline float TanTheta(Vector3f w) {
//    return SinTheta(w) / CosTheta(w);
//}
//
//inline float CosPhi(const Vector3f w)
//{
//    float sinTheta = SinTheta(w);
//    return (sinTheta == 0) ? 1.0f : ffclamp(w.x() / sinTheta, -1.0f, 1.0f);
//}
//
//inline float Cos2Phi(const Vector3f w)
//{
//    float cosPhi = CosPhi(w);
//    return cosPhi * cosPhi;
//}
//
//float SinPhi(const Vector3f w)
//{
//    float sinTheta = SinTheta(w);
//    return (sinTheta == 0) ? 1.0f : ffclamp(w.z() / sinTheta, -1.0f, 1.0f);
//}
//
//inline float Sin2Phi(const Vector3f w)
//{
//    float sinPhi = SinPhi(w);
//    return sinPhi * sinPhi;
//}
//
//template <typename T>
//inline T Lerp(T a, T b, float t) {
//    return (1.0f - t) * a + t * b;
//}
//
//
//inline Vector3f random_in_unit_sphere() {
//    Vector3f p;
//    do {
//        p = 2.0 * Vector3f(drand48(), drand48(), drand48()) - Vector3f(1,1,1);
//    } while (p.squareMagnitude() >= 1.0);
//    return p;
//}
//
//inline Vector3f random_cosine_direction() {
//    float r1 = drand48();
//    float r2 = drand48();
//    float z = sqrt(1-r2);
//    float phi = 2*M_PI*r1;
//    float x = cos(phi)*2*sqrt(r2);
//    float y = sin(phi)*2*sqrt(r2);
//    return Vector3f(x,y,z);
//}
//
////=========================================================================================================================
//
//float SeparableSmithGGXG1(const Vector3f& w, const Vector3f& wm, float ax, float ay)
//{
//    // JSTODO - Why did I have this here?
//    //float dotHW = Dot(w, wm);
//    //if (dotHW <= 0.0f) {
//    //    return 0.0f;
//    //}
//
//    float absTanTheta = fabs(TanTheta(w));
//    if(isinf(absTanTheta)) {
//        return 0.0f;
//    }
//
//    float a = sqrtf(Cos2Phi(w) * ax * ax + Sin2Phi(w) * ay * ay);
//    float a2Tan2Theta = Square(a * absTanTheta);
//
//    float lambda = 0.5f * (-1.0f + sqrtf(1.0f + a2Tan2Theta));
//    return 1.0f / (1.0f + lambda);
//}
//
//float SeparableSmithGGXG1(const Vector3f& w, float a)
//{
//    float a2 = a * a;
//    float absDotNV = AbsCosTheta(w);
//
//    return 2.0f / (1.0f + sqrtf(a2 + (1 - a2) * absDotNV * absDotNV));
//}
//
//float GgxAnisotropicD(const Vector3f& wm, float ax, float ay)
//{
//    float dotHX2 = Square(wm.x());
//    float dotHY2 = Square(wm.z());
//    float cos2Theta = Cos2Theta(wm);
//    float ax2 = Square(ax);
//    float ay2 = Square(ay);
//
//    return 1.0f / (M_PI * ax * ay * Square(dotHX2 / ax2 + dotHY2 / ay2 + cos2Theta));
//}
//
//float GgxVndfAnisotropicPdf(const Vector3f& wi, const Vector3f& wm, const Vector3f& wo, float ax, float ay)
//{
//    float absDotNL = AbsCosTheta(wi);
//    float absDotLH = fabs(Vector3f::dotProduct(wm, wi));
//
//    float G1 = SeparableSmithGGXG1(wo, wm, ax, ay);
//    float D = GgxAnisotropicD(wm, ax, ay);
//
//    return G1 * absDotLH * D / absDotNL;
//}
//
//void GgxVndfAnisotropicPdf(const Vector3f& wi, const Vector3f& wm, const Vector3f& wo, float ax, float ay,
//                           float& forwardPdfW, float& reversePdfW)
//{
//    float D = GgxAnisotropicD(wm, ax, ay);
//
//    float absDotNL = AbsCosTheta(wi);
//    float absDotHL = fabs(Vector3f::dotProduct(wm, wi));
//    float G1v = SeparableSmithGGXG1(wo, wm, ax, ay);
//    forwardPdfW = G1v * absDotHL * D / absDotNL;
//
//    float absDotNV = AbsCosTheta(wo);
//    float absDotHV = fabs(Vector3f::dotProduct(wm, wo));
//    float G1l = SeparableSmithGGXG1(wi, wm, ax, ay);
//    reversePdfW = G1l * absDotHV * D / absDotNV;
//}
//
//Vector3f SampleGgxVndfAnisotropic(const Vector3f& wo, float ax, float ay, float u1, float u2)
//{
//    // -- Stretch the view vector so we are sampling as though roughness==1
//    Vector3f v = Vector3f(wo.x() * ax, wo.y(), wo.z() * ay).normalized();
//
//    // -- Build an orthonormal basis with v, t1, and t2
//    Vector3f t1 = (v.y() < 0.9999f) ? Vector3f::crossProduct(v, Vector3f(0,1,0)).normalized() : Vector3f(1,0,0);
//    Vector3f t2 = Vector3f::crossProduct(t1, v);
//
//    // -- Choose a point on a disk with each half of the disk weighted proportionally to its projection onto direction v
//    float a = 1.0f / (1.0f + v.y());
//    float r = sqrtf(u1);
//    float phi = (u2 < a) ? (u2 / a) * M_PI : M_PI + (u2 - a) / (1.0f - a) * M_PI;
//    float p1 = r * cosf(phi);
//    float p2 = r * sinf(phi) * ((u2 < a) ? 1.0f : v.y());
//
//    // -- Calculate the normal in this stretched tangent space
//    Vector3f n = p1 * t1 + p2 * t2 + sqrtf(ffmax(0.0f, 1.0f - p1 * p1 - p2 * p2)) * v;
//
//    // -- unstretch and normalize the normal
//    return Vector3f(ax * n.x(), n.y(), ay * n.z()).normalized();
//}
//
////=========================================================================================================================
//
//
//bool disney::scatter(const Ray& ray_in, const SurfaceInteraction& hrec, scatter_record& srec) const {
//    srec.is_specular = false;
//    srec.attenuation = albedo->value(hrec.u, hrec.v, hrec.p);
//    srec.pdf_ptr = new cosine_pdf(hrec.normal);
//    return true;
//};
//
//float disney::scattering_pdf(const Ray& ray_in, const SurfaceInteraction& rec, Ray& scattered) const {
//    float cosine = Vector3f::dotProduct(rec.normal, scattered.direction().normalized());
//    if (cosine < 0) {
//        cosine = 0;
//    }
//    return cosine / M_PI;
//};
//
//
////=============================================================================================================================
//static void CalculateLobePdfs(const SurfaceParameters& surface, float& pSpecular, float& pDiffuse, float& pClearcoat, float& pSpecTrans) {
//    float metallicBRDF   = surface.metallic;
//    float specularBSDF   = (1.0f - surface.metallic) * surface.specTrans;
//    float dielectricBRDF = (1.0f - surface.specTrans) * (1.0f - surface.metallic);
//
//    float specularWeight     = metallicBRDF + dielectricBRDF;
//    float transmissionWeight = specularBSDF;
//    float diffuseWeight      = dielectricBRDF;
//    float clearcoatWeight    = 1.0f * Saturate(surface.clearcoat);
//
//    float norm = 1.0f / (specularWeight + transmissionWeight + diffuseWeight + clearcoatWeight);
//
//    pSpecular  = specularWeight     * norm;
//    pSpecTrans = transmissionWeight * norm;
//    pDiffuse   = diffuseWeight      * norm;
//    pClearcoat = clearcoatWeight    * norm;
//}
//
////=============================================================================================================================
//static float ThinTransmissionRoughness(float ior, float roughness) {
//    // -- Disney scales by (.65 * eta - .35) based on figure 15 of the 2015 PBR course notes. Based on their figure the results
//    // -- match a geometrically thin solid fairly well but it is odd to me that roughness is decreased until an IOR of just
//    // -- over 2.
//    return Saturate((0.65f * ior - 0.35f) * roughness);
//}
//
////=============================================================================================================================
//static void CalculateAnisotropicParams(float roughness, float anisotropic, float& ax, float& ay) {
//    float aspect = sqrtf(1.0f - 0.9f * anisotropic);
//    ax = ffmax(0.001f, Square(roughness) / aspect);
//    ay = ffmax(0.001f, Square(roughness) * aspect);
//}
//
////=============================================================================================================================
//static Vector3f CalculateTint(Vector3f baseColor)
//{
//    // -- The color tint is never mentioned in the SIGGRAPH presentations as far as I recall but it was done in the BRDF
//    // -- Explorer so I'll replicate that here.
//    float luminance = Vector3f::dotProduct(Vector3f(0.3f, 0.6f, 1.0f), baseColor);
//    return (luminance > 0.0f) ? baseColor * (1.0f / luminance) : Vector3f(1.0);
//}
//
////=============================================================================================================================
//// -- "generalized" Trowbridge-Reitz curve ungeneralized with a hard-coded exponent of 1
//static float GTR1(float absDotHL, float a)
//{
//    if(a >= 1) {
//        return M_1_PI;
//    }
//
//    float a2 = a * a;
//    return (a2 - 1.0f) / (M_PI * log2(a2) * (1.0f + (a2 - 1.0f) * absDotHL * absDotHL));
//}
//
////=============================================================================================================================
//static float EvaluateDisneyClearcoat(float clearcoat, float alpha, const Vector3f& wo, const Vector3f& wm, const Vector3f& wi, float& fPdfW, float& rPdfW)
//{
//    if(clearcoat <= 0.0f) {
//        return 0.0f;
//    }
//
//    float absDotNH = AbsCosTheta(wm);
//    float absDotNL = AbsCosTheta(wi);
//    float absDotNV = AbsCosTheta(wo);
//    float dotHL = Vector3f::dotProduct(wm, wi);
//
//    float d = GTR1(absDotNH, Lerp(0.1f, 0.001f, alpha));
//    float f = Fresnel::Schlick(0.04f, dotHL);
//    float gl = SeparableSmithGGXG1(wi, 0.25f);
//    float gv = SeparableSmithGGXG1(wo, 0.25f);
//
//    fPdfW = d / (4.0f * fabs(Vector3f::dotProduct(wo, wm)));
//    rPdfW = d / (4.0f * fabs(Vector3f::dotProduct(wi, wm)));
//
//    return 0.25f * clearcoat * d * f * gl * gv;
//}
//
////=============================================================================================================================
//static Vector3f EvaluateSheen(const SurfaceParameters& surface, const Vector3f& wo, const Vector3f& wm, const Vector3f& wi)
//{
//    if(surface.sheen <= 0.0f) {
//        return Vector3f(0,0,0);
//    }
//
//    float dotHL = fabs(Vector3f::dotProduct(wm, wi));
//
//    Vector3f tint = CalculateTint(surface.baseColor);
//    return surface.sheen * Lerp(Vector3f(1.0), tint, surface.sheenTint) * Fresnel::SchlickWeight(dotHL);
//}
//
////=============================================================================================================================
//static Vector3f DisneyFresnel(const SurfaceParameters& surface, const Vector3f& wo, const Vector3f& wm, const Vector3f& wi)
//{
//    float dotHV = Vector3f::dotProduct(wm, wo);
//
//    Vector3f tint = CalculateTint(surface.baseColor);
//
//    // -- See section 3.1 and 3.2 of the 2015 PBR presentation + the Disney BRDF explorer (which does their 2012 remapping
//    // -- rather than the SchlickR0FromRelativeIOR seen here but they mentioned the switch in 3.2).
//    Vector3f R0 = Fresnel::SchlickR0FromRelativeIOR(surface.relativeIOR) * Lerp(Vector3f(1.0), tint, surface.specularTint);
//             R0 = Lerp(R0, surface.baseColor, surface.metallic);
//
//    float dielectricFresnel = Fresnel::Dielectric(dotHV, 1.0f, surface.ior);
//    Vector3f metallicFresnel = Fresnel::Schlick(R0, Vector3f::dotProduct(wi, wm));
//
//    return Lerp(Vector3f(dielectricFresnel), metallicFresnel, surface.metallic);
//}
//
////=============================================================================================================================
//static Vector3f EvaluateDisneyBRDF(const SurfaceParameters& surface, const Vector3f& wo, const Vector3f& wm, const Vector3f& wi,
//                                 float& fPdf, float& rPdf)
//{
//    fPdf = 0.0f;
//    rPdf = 0.0f;
//
//    float dotNL = CosTheta(wi);
//    float dotNV = CosTheta(wo);
//    if(dotNL <= 0.0f || dotNV <= 0.0f) {
//        return Vector3f(0.0);
//    }
//
//    float ax, ay;
//    CalculateAnisotropicParams(surface.roughness, surface.anisotropic, ax, ay);
//
//    float d = GgxAnisotropicD(wm, ax, ay);
//    float gl = SeparableSmithGGXG1(wi, wm, ax, ay);
//    float gv = SeparableSmithGGXG1(wo, wm, ax, ay);
//
//    Vector3f f = DisneyFresnel(surface, wo, wm, wi);
//
//    GgxVndfAnisotropicPdf(wi, wm, wo, ax, ay, fPdf, rPdf);
//    fPdf *= (1.0f / (4 * fabs(Vector3f::dotProduct(wo, wm))));
//    rPdf *= (1.0f / (4 * fabs(Vector3f::dotProduct(wi, wm))));
//
//    return d * gl * gv * f / (4.0f * dotNL * dotNV);
//}
//
////=============================================================================================================================
//static bool SampleDisneyBRDF(Sampler* sampler, const SurfaceParameters& surface, Vector3f v, BsdfSample& sample)
//{
//    Vector3f wo = Normalize(MatrixMultiply(v, surface.worldToTangent));
//
//    // -- Calculate Anisotropic params
//    float ax, ay;
//    CalculateAnisotropicParams(surface.roughness, surface.anisotropic, ax, ay);
//
//    // -- Sample visible distribution of normals
//    float r0 = sampler->get1D();
//    float r1 = sampler->get1D();
//    Vector3f wm = SampleGgxVndfAnisotropic(wo, ax, ay, r0, r1);
//
//    // -- Reflect over wm
//    Vector3f wi = Vector3f::reflect(wm, wo).normalized();
//    if(CosTheta(wi) <= 0.0f) {
//        sample.forwardPdfW = 0.0f;
//        sample.reversePdfW = 0.0f;
//        sample.reflectance = Vector3f(0.0);
//        sample.wi = Vector3f(0.0);
//        return false;
//    }
//
//    // -- Fresnel term for this lobe is complicated since we're blending with both the metallic and the specularTint
//    // -- parameters plus we must take the IOR into account for dielectrics
//    Vector3f F = DisneyFresnel(surface, wo, wm, wi);
//
//    // -- Since we're sampling the distribution of visible normals the pdf cancels out with a number of other terms.
//    // -- We are left with the weight G2(wi, wo, wm) / G1(wi, wm) and since Disney uses a separable masking function
//    // -- we get G1(wi, wm) * G1(wo, wm) / G1(wi, wm) = G1(wo, wm) as our weight.
//    float G1v = SeparableSmithGGXG1(wo, wm, ax, ay);
//    Vector3f specular = G1v * F;
//
//    sample.flags = SurfaceEventFlags::eScatterEvent;
//    sample.reflectance = specular;
//    sample.wi = Normalize(MatrixMultiply(wi, MatrixTranspose(surface.worldToTangent)));
//    GgxVndfAnisotropicPdf(wi, wm, wo, ax, ay, sample.forwardPdfW, sample.reversePdfW);
//
//    sample.forwardPdfW *= (1.0f / (4 * fabs(Vector3f::dotProduct(wo, wm))));
//    sample.reversePdfW *= (1.0f / (4 * fabs(Vector3f::dotProduct(wi, wm))));
//
//    return true;
//}
//
////=============================================================================================================================
//static Vector3f EvaluateDisneySpecTransmission(const SurfaceParameters& surface, const Vector3f& wo, const Vector3f& wm,
//                                             const Vector3f& wi, float ax, float ay, bool thin)
//{
//    float relativeIor = surface.relativeIOR;
//    float n2 = relativeIor * relativeIor;
//
//    float absDotNL = AbsCosTheta(wi);
//    float absDotNV = AbsCosTheta(wo);
//    float dotHL = Vector3f::dotProduct(wm, wi);
//    float dotHV = Vector3f::dotProduct(wm, wo);
//    float absDotHL = fabs(dotHL);
//    float absDotHV = fabs(dotHV);
//
//    float d = GgxAnisotropicD(wm, ax, ay);
//    float gl = SeparableSmithGGXG1(wi, wm, ax, ay);
//    float gv = SeparableSmithGGXG1(wo, wm, ax, ay);
//
//    float f = Fresnel::Dielectric(dotHV, 1.0f, surface.ior);
//
//    Vector3f color;
//    if (thin)
//        color = surface.baseColor.sqrt();
//    else
//        color = surface.baseColor;
//
//    // Note that we are intentionally leaving out the 1/n2 spreading factor since for VCM we will be evaluating particles with
//    // this. That means we'll need to model the air-[other medium] transmission if we ever place the camera inside a non-air
//    // medium.
//    float c = (absDotHL * absDotHV) / (absDotNL * absDotNV);
//    float t = (n2 / Square(dotHL + relativeIor * dotHV));
//    return color * c * t * (1.0f - f) * gl * gv * d;
//}
//
////=============================================================================================================================
//static float EvaluateDisneyRetroDiffuse(const SurfaceParameters& surface, const Vector3f& wo, const Vector3f& wm, const Vector3f& wi)
//{
//    float dotNL = AbsCosTheta(wi);
//    float dotNV = AbsCosTheta(wo);
//
//    float roughness = surface.roughness * surface.roughness;
//
//    float rr = 0.5f + 2.0f * dotNL * dotNL * roughness;
//    float fl = Fresnel::SchlickWeight(dotNL);
//    float fv = Fresnel::SchlickWeight(dotNV);
//
//    return rr * (fl + fv + fl * fv * (rr - 1.0f));
//}
//
////=============================================================================================================================
//static float EvaluateDisneyDiffuse(const SurfaceParameters& surface, const Vector3f& wo, const Vector3f& wm, const Vector3f& wi, bool thin)
//{
//    float dotNL = AbsCosTheta(wi);
//    float dotNV = AbsCosTheta(wo);
//
//    float fl = Fresnel::SchlickWeight(dotNL);
//    float fv = Fresnel::SchlickWeight(dotNV);
//
//    float hanrahanKrueger = 0.0f;
//
//    if(thin && surface.flatness > 0.0f) {
//        float roughness = surface.roughness * surface.roughness;
//
//        float dotHL = Vector3f::dotProduct(wm, wi);
//        float fss90 = dotHL * dotHL * roughness;
//        float fss = Lerp(1.0f, fss90, fl) * Lerp(1.0f, fss90, fv);
//
//        float ss = 1.25f * (fss * (1.0f / (dotNL + dotNV) - 0.5f) + 0.5f);
//        hanrahanKrueger = ss;
//    }
//
//    float lambert = 1.0f;
//    float retro = EvaluateDisneyRetroDiffuse(surface, wo, wm, wi);
//    float subsurfaceApprox = Lerp(lambert, hanrahanKrueger, thin ? surface.flatness : 0.0f);
//
//    return M_1_PI * (retro + subsurfaceApprox * (1.0f - 0.5f * fl) * (1.0f - 0.5f * fv));
//}
//
////=============================================================================================================================
//static bool SampleDisneyClearcoat(Sampler* sampler, const SurfaceParameters& surface, const Vector3f& v, BsdfSample& sample)
//{
//    Vector3f wo = Normalize(MatrixMultiply(v, surface.worldToTangent));
//
//    float a = 0.25f;
//    float a2 = a * a;
//
//    float r0 = sampler->get1D();
//    float r1 = sampler->get1D();
//    float cosTheta = sqrtf(ffmax<float>(0, (1.0f - powf(a2, 1.0f - r0)) / (1.0f - a2)));
//    float sinTheta = sqrtf(ffmax<float>(0, 1.0f - cosTheta * cosTheta));
//    float phi = M_PI * 2 * r1;
//
//    Vector3f wm = Vector3f(sinTheta * cosf(phi), cosTheta, sinTheta * sinf(phi));
//    if(Vector3f::dotProduct(wm, wo) < 0.0f) {
//        wm = -wm;
//    }
//
//    float3 wi = Reflect(wm, wo);
//    if(Dot(wi, wo) < 0.0f) {
//        return false;
//    }
//
//    float clearcoatWeight = surface.clearcoat;
//    float clearcoatGloss = surface.clearcoatGloss;
//
//    float dotNH = CosTheta(wm);
//    float dotLH = Dot(wm, wi);
//
//    float d = GTR1(Absf(dotNH), Lerp(0.1f, 0.001f, clearcoatGloss));
//    float f = Fresnel::Schlick(0.04f, dotLH);
//    float g = SeparableSmithGGXG1(wi, 0.25f) * SeparableSmithGGXG1(wo, 0.25f);
//
//    float fPdf = d / (4.0f * Dot(wo, wm));
//
//    sample.reflectance = float3(0.25f * clearcoatWeight * g * f * d) / fPdf;
//    sample.wi = Normalize(MatrixMultiply(wi, MatrixTranspose(surface.worldToTangent)));
//    sample.forwardPdfW = fPdf;
//    sample.reversePdfW = d / (4.0f * Dot(wi, wm));
//
//    return true;
//}
//
////=============================================================================================================================
//static Vector3f CalculateExtinction(float3 apparantColor, float scatterDistance)
//{
//    float3 a = apparantColor;
//    float3 s = float3(1.9f) - a + 3.5f * (a - float3(0.8f)) * (a - float3(0.8f));
//
//    return 1.0f / (s * scatterDistance);
//}
//
////=============================================================================================================================
//static bool SampleDisneySpecTransmission(Sampler* sampler, const SurfaceParameters& surface, Vector3f v, bool thin, BsdfSample& sample)
//{
//    float3 wo = MatrixMultiply(v, surface.worldToTangent);
//    if(CosTheta(wo) == 0.0) {
//        sample.forwardPdfW = 0.0f;
//        sample.reversePdfW = 0.0f;
//        sample.reflectance = float3::Zero_;
//        sample.wi = float3::Zero_;
//        return false;
//    }
//
//    // -- Scale roughness based on IOR
//    float rscaled = thin ? ThinTransmissionRoughness(surface.ior, surface.roughness) : surface.roughness;
//
//    float tax, tay;
//    CalculateAnisotropicParams(rscaled, surface.anisotropic, tax, tay);
//
//    // -- Sample visible distribution of normals
//    float r0 = sampler->get1D();
//    float r1 = sampler->get1D();
//    Vector3f wm = SampleGgxVndfAnisotropic(wo, tax, tay, r0, r1);
//
//    float dotVH = Dot(wo, wm);
//    if(wm.y < 0.0f) {
//        dotVH = -dotVH;
//    }
//
//    float ni = wo.y > 0.0f ? 1.0f : surface.ior;
//    float nt = wo.y > 0.0f ? surface.ior : 1.0f;
//    float relativeIOR = ni / nt;
//
//    // -- Disney uses the full dielectric Fresnel equation for transmission. We also importance sample F to switch between
//    // -- refraction and reflection at glancing angles.
//    float F = Fresnel::Dielectric(dotVH, 1.0f, surface.ior);
//
//    // -- Since we're sampling the distribution of visible normals the pdf cancels out with a number of other terms.
//    // -- We are left with the weight G2(wi, wo, wm) / G1(wi, wm) and since Disney uses a separable masking function
//    // -- we get G1(wi, wm) * G1(wo, wm) / G1(wi, wm) = G1(wo, wm) as our weight.
//    float G1v = SeparableSmithGGXG1(wo, wm, tax, tay);
//
//    float pdf;
//
//    float3 wi;
//    if(sampler->get1D() <= F) {
//        wi = Normalize(Reflect(wm, wo));
//
//        sample.flags = SurfaceEventFlags::eScatterEvent;
//        sample.reflectance = G1v * surface.baseColor;
//
//        float jacobian = (4 * AbsDot(wo, wm));
//        pdf = F / jacobian;
//    }
//    else {
//        if(thin) {
//            // -- When the surface is thin so it refracts into and then out of the surface during this shading event.
//            // -- So the ray is just reflected then flipped and we use the sqrt of the surface color.
//            wi = Reflect(wm, wo);
//            wi.y = -wi.y;
//            sample.reflectance = G1v * Sqrt(surface.baseColor);
//
//            // -- Since this is a thin surface we are not ending up inside of a volume so we treat this as a scatter event.
//            sample.flags = SurfaceEventFlags::eScatterEvent;
//        }
//        else {
//            if(Transmit(wm, wo, relativeIOR, wi)) {
//                sample.flags = SurfaceEventFlags::eTransmissionEvent;
//                sample.medium.phaseFunction = dotVH > 0.0f ? MediumPhaseFunction::eIsotropic : MediumPhaseFunction::eVacuum;
//                sample.medium.extinction = CalculateExtinction(surface.transmittanceColor, surface.scatterDistance);
//            }
//            else {
//                sample.flags = SurfaceEventFlags::eScatterEvent;
//                wi = Reflect(wm, wo);
//            }
//
//            sample.reflectance = G1v * surface.baseColor;
//        }
//
//        wi = Normalize(wi);
//
//        float dotLH = Absf(Dot(wi, wm));
//        float jacobian = dotLH / (Square(dotLH + surface.relativeIOR * dotVH));
//        pdf = (1.0f - F) / jacobian;
//    }
//
//    if(CosTheta(wi) == 0.0f) {
//        sample.forwardPdfW = 0.0f;
//        sample.reversePdfW = 0.0f;
//        sample.reflectance = float3::Zero_;
//        sample.wi = float3::Zero_;
//        return false;
//    }
//
//    if(surface.roughness < 0.01f) {
//        sample.flags |= SurfaceEventFlags::eDiracEvent;
//    }
//
//    // -- calculate pdf terms
//    GgxVndfAnisotropicPdf(wi, wm, wo, tax, tay, sample.forwardPdfW, sample.reversePdfW);
//    sample.forwardPdfW *= pdf;
//    sample.reversePdfW *= pdf;
//
//    // -- convert wi back to world space
//    sample.wi = Normalize(MatrixMultiply(wi, MatrixTranspose(surface.worldToTangent)));
//
//    return true;
//}
//
////=============================================================================================================================
//static Vector3f SampleCosineWeightedHemisphere(float r0, float r1)
//{
//    float r = sqrtf(r0);
//    float theta = TwoPi_ * r1;
//
//    return Vector3f(r * Cosf(theta), sqrtf(Max(0.0f, 1 - r0)), r * Sinf(theta));
//}
//
////=============================================================================================================================
//static bool SampleDisneyDiffuse(Sampler* sampler, const SurfaceParameters& surface, Vector3f v, bool thin, BsdfSample& sample)
//{
//    float3 wo = MatrixMultiply(v, surface.worldToTangent);
//
//    float sign = Sign(CosTheta(wo));
//
//    // -- Sample cosine lobe
//    float r0 = sampler->get1D();
//    float r1 = sampler->get1D();
//    float3 wi = sign * SampleCosineWeightedHemisphere(r0, r1);
//    float3 wm = Normalize(wi + wo);
//
//    float dotNL = CosTheta(wi);
//    if(dotNL == 0.0f) {
//        sample.forwardPdfW = 0.0f;
//        sample.reversePdfW = 0.0f;
//        sample.reflectance = float3::Zero_;
//        sample.wi = float3::Zero_;
//        return false;
//    }
//
//    float dotNV = CosTheta(wo);
//
//    float pdf;
//
//    SurfaceEventFlags eventType = SurfaceEventFlags::eScatterEvent;
//
//    float3 color = surface.baseColor;
//
//    float p = sampler->get1D();
//    if(p <= surface.diffTrans) {
//        wi = -wi;
//        pdf = surface.diffTrans;
//
//        if(thin)
//            color = Sqrt(color);
//        else {
//            eventType = SurfaceEventFlags::eTransmissionEvent;
//
//            sample.medium.phaseFunction = MediumPhaseFunction::eIsotropic;
//            sample.medium.extinction = CalculateExtinction(surface.transmittanceColor, surface.scatterDistance);
//        }
//    }
//    else {
//        pdf = (1.0f - surface.diffTrans);
//    }
//
//    float3 sheen = EvaluateSheen(surface, wo, wm, wi);
//
//    float diffuse = EvaluateDisneyDiffuse(surface, wo, wm, wi, thin);
//
//    Assert_(pdf > 0.0f);
//    sample.reflectance = sheen + color * (diffuse / pdf);
//    sample.wi = Normalize(MatrixMultiply(wi, MatrixTranspose(surface.worldToTangent)));
//    sample.forwardPdfW = Absf(dotNL) * pdf;
//    sample.reversePdfW = Absf(dotNV) * pdf;
//    sample.flags = eventType;
//    return true;
//}
//
////=============================================================================================================================
//Vector3f EvaluateDisney(const SurfaceParameters& surface, Vector3f v, Vector3f l, bool thin, float& forwardPdf, float& reversePdf)
//{
//    float3 wo = Normalize(MatrixMultiply(v, surface.worldToTangent));
//    float3 wi = Normalize(MatrixMultiply(l, surface.worldToTangent));
//    float3 wm = Normalize(wo + wi);
//
//    float dotNV = CosTheta(wo);
//    float dotNL = CosTheta(wi);
//
//    float3 reflectance = float3::Zero_;
//    forwardPdf = 0.0f;
//    reversePdf = 0.0f;
//
//    float pBRDF, pDiffuse, pClearcoat, pSpecTrans;
//    CalculateLobePdfs(surface, pBRDF, pDiffuse, pClearcoat, pSpecTrans);
//
//    float metallic = surface.metallic;
//    float specTrans = surface.specTrans;
//
//    // calculate all of the anisotropic params
//    float ax, ay;
//    CalculateAnisotropicParams(surface.roughness, surface.anisotropic, ax, ay);
//
//    float diffuseWeight = (1.0f - metallic) * (1.0f - specTrans);
//    float transWeight   = (1.0f - metallic) * specTrans;
//
//    // -- Clearcoat
//    bool upperHemisphere = dotNL > 0.0f && dotNV > 0.0f;
//    if(upperHemisphere && surface.clearcoat > 0.0f) {
//
//        float forwardClearcoatPdfW;
//        float reverseClearcoatPdfW;
//
//        float clearcoat = EvaluateDisneyClearcoat(surface.clearcoat, surface.clearcoatGloss, wo, wm, wi,
//                                                  forwardClearcoatPdfW, reverseClearcoatPdfW);
//        reflectance += float3(clearcoat);
//        forwardPdf += pClearcoat * forwardClearcoatPdfW;
//        reversePdf += pClearcoat * reverseClearcoatPdfW;
//    }
//
//    // -- Diffuse
//    if(diffuseWeight > 0.0f) {
//        float forwardDiffusePdfW = AbsCosTheta(wi);
//        float reverseDiffusePdfW = AbsCosTheta(wo);
//        float diffuse = EvaluateDisneyDiffuse(surface, wo, wm, wi, thin);
//
//        float3 sheen = EvaluateSheen(surface, wo, wm, wi);
//
//        reflectance += diffuseWeight * (diffuse * surface.baseColor + sheen);
//
//        forwardPdf += pDiffuse * forwardDiffusePdfW;
//        reversePdf += pDiffuse * reverseDiffusePdfW;
//    }
//
//    // -- transmission
//    if(transWeight > 0.0f) {
//
//        // Scale roughness based on IOR (Burley 2015, Figure 15).
//        float rscaled = thin ? ThinTransmissionRoughness(surface.ior, surface.roughness) : surface.roughness;
//        float tax, tay;
//        CalculateAnisotropicParams(rscaled, surface.anisotropic, tax, tay);
//
//        float3 transmission = EvaluateDisneySpecTransmission(surface, wo, wm, wi, tax, tay, thin);
//        reflectance += transWeight * transmission;
//
//        float forwardTransmissivePdfW;
//        float reverseTransmissivePdfW;
//        GgxVndfAnisotropicPdf(wi, wm, wo, tax, tay, forwardTransmissivePdfW, reverseTransmissivePdfW);
//
//        float dotLH = Dot(wm, wi);
//        float dotVH = Dot(wm, wo);
//        forwardPdf += pSpecTrans * forwardTransmissivePdfW / (Square(dotLH + surface.relativeIOR * dotVH));
//        reversePdf += pSpecTrans * reverseTransmissivePdfW / (Square(dotVH + surface.relativeIOR * dotLH));
//    }
//
//    // -- specular
//    if(upperHemisphere) {
//        float forwardMetallicPdfW;
//        float reverseMetallicPdfW;
//        float3 specular = EvaluateDisneyBRDF(surface, wo, wm, wi, forwardMetallicPdfW, reverseMetallicPdfW);
//
//        reflectance += specular;
//        forwardPdf += pBRDF * forwardMetallicPdfW / (4 * AbsDot(wo, wm));
//        reversePdf += pBRDF * reverseMetallicPdfW / (4 * AbsDot(wi, wm));
//    }
//
//    reflectance = reflectance * Absf(dotNL);
//
//    return reflectance;
//}
//
////=============================================================================================================================
//bool SampleDisney(Sampler* sampler, const SurfaceParameters& surface, Vector3f v, bool thin, BsdfSample& sample)
//{
//    float pSpecular;
//    float pDiffuse;
//    float pClearcoat;
//    float pTransmission;
//    CalculateLobePdfs(surface, pSpecular, pDiffuse, pClearcoat, pTransmission);
//
//    bool success = false;
//
//    float pLobe = 0.0f;
//    float p = sampler->get1D();
//    if(p <= pSpecular) {
//        success = SampleDisneyBRDF(sampler, surface, v, sample);
//        pLobe = pSpecular;
//    }
//    else if(p > pSpecular && p <= (pSpecular + pClearcoat)) {
//        success = SampleDisneyClearcoat(sampler, surface, v, sample);
//        pLobe = pClearcoat;
//    }
//    else if(p > pSpecular + pClearcoat && p <= (pSpecular + pClearcoat + pDiffuse)) {
//        success = SampleDisneyDiffuse(sampler, surface, v, thin, sample);
//        pLobe = pDiffuse;
//    }
//    else if(pTransmission >= 0.0f) {
//        success = SampleDisneySpecTransmission(sampler, surface, v, thin, sample);
//        pLobe = pTransmission;
//    }
//    else {
//        // -- Make sure we notice if this is occurring.
//        sample.reflectance = float3(1000000.0f, 0.0f, 0.0f);
//        sample.forwardPdfW = 0.000000001f;
//        sample.reversePdfW = 0.000000001f;
//    }
//
//    if(pLobe > 0.0f) {
//        sample.reflectance = sample.reflectance * (1.0f / pLobe);
//        sample.forwardPdfW *= pLobe;
//        sample.reversePdfW *= pLobe;
//    }
//
//    return success;
//}
