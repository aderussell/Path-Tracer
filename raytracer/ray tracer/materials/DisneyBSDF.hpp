//
//  DisneyBSDF.hpp
//  raytracer
//
//  Created by Adrian Russell on 04/04/2020.
//  Copyright © 2020 Adrian Russell. All rights reserved.
//

#ifndef DisneyBSDF_hpp
#define DisneyBSDF_hpp

#include "Material.hpp"
#include "texture.hpp"
#include "Sampler.hpp"

//enum ShaderType {
//    eDisneyThin,
//    eDisneySolid,
//    eDiracTransparent,
//
//    eShaderCount
//};
//
//struct SurfaceParameters {
//    Vector3f position;
//    Vector3f perturbedNormal;
//    Vector3f worldToTangent;
//    float error;
//
//    Vector3f view;
//
//    Vector3f baseColor;
//    Vector3f transmittanceColor;
//    float sheen;
//    float sheenTint;
//    float clearcoat;
//    float clearcoatGloss;
//    float metallic;
//    float specTrans;
//    float diffTrans;
//    float flatness;
//    float anisotropic;
//    float relativeIOR;
//    float specularTint;
//    float roughness;
//    float scatterDistance;
//
//    float ior;
//
//    // -- material layer info
//    ShaderType shader;
//    uint32_t materialFlags;
//};
//
//class disney: public material {
//
//public:
//    disney(texture* a) : albedo(a) {}
//
//    virtual bool scatter(const Ray& ray_in, const SurfaceInteraction& hrec, scatter_record& srec) const;
//    virtual float scattering_pdf(const Ray& ray_in, const SurfaceInteraction& rec, Ray& scattered) const;
//
//
//    // -- BSDF evaluation for next event estimation
//    Vector3f EvaluateDisney(const SurfaceParameters& surface, Vector3f v, Vector3f l, bool thin, float& forwardPdf, float& reversePdf);
//
//    // -- Shaders
//    bool SampleDisney(Sampler* sampler, const SurfaceParameters& surface, Vector3f v, bool thin, BsdfSample& sample);
//
//private:
//    texture *albedo;
//};


#endif /* DisneyBSDF_hpp */
