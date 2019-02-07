//
//  obj_loader.hpp
//  raytracer
//
//  Created by Adrian Russell on 26/12/2018.
//  Copyright © 2018 Adrian Russell. All rights reserved.
//

#ifndef obj_loader_hpp
#define obj_loader_hpp

#include <string>
#include <map>
#include <unordered_map>
#include <vector>
#include "Vector3.hpp"
#include "Material.hpp"
#include "triangle.hpp"

#include "hitable_list.hpp"
#include "bvh.hpp"

struct SimpleVertex
{
    Vector3 Pos;
    Vector3 TexUV;
    Vector3 VecNormal;
};

class MeshMaterial {
public:
    std::string name;
    
    Color ambient;
    Color diffuse;
    Color specular;
    
    std::string ambientTexture;
    std::string diffuseTexture;
    std::string specularTexture;
    
    float specularExponent;
    
    float dissolved = 1.0;
    
    typedef enum {
        constant = 0, // Kd color
        diffuse_, // Lambertian shading
        blinn,    // actually Blinn-Phong diffuse and specular combined
        reflective, // Blinn-Phong plus reflection
        transparent, // Blinn-Phong plus transparency
        fresnelReflection, // Blinn-Phong plus Fresnel reflection
        transparentNoReflection,
        transparentReflection,
        reflectionNoRayTrace,
        transparentNoRayTrace,
        castShadows
    } Illumination;
    
    Illumination illuminationModel;
};


class Mesh {
public:
    std::vector<SimpleVertex> vertices;
    std::vector<int> indices;
    
    hitable* create_hitable(material *mat) {
        
        int j = 0;
        int size = (int)indices.size() / 3;
        hitable **h = new hitable*[size];
        
        for (int i = 0; i < indices.size(); i+=3) {
            SimpleVertex s1 = vertices[indices[i+0]];
            SimpleVertex s2 = vertices[indices[i+1]];
            SimpleVertex s3 = vertices[indices[i+2]];
            
            triangle *t = new triangle_with_normals(s1.Pos, s2.Pos, s3.Pos, mat, s1.VecNormal, s2.VecNormal, s3.VecNormal);
            //triangle *t = new triangle(s1.Pos, s2.Pos, s3.Pos, mat);
            h[j++] = t;
        }
        
        //return new hitable_list(h, size);
        return new bvh_node(h, size);
    }
};

class MeshGroup {
public:
    std::vector<SimpleVertex> vertices;
    std::vector<int> indices;
    std::vector<int> startOfRange;
    std::vector<int> startOfGroup;
    std::vector<std::string> materialAtRange;
    
    std::unordered_map<std::wstring, MeshMaterial *> materials;
    
    void addMaterials(std::unordered_map<std::string, MeshMaterial *>& materials) {
        for (auto kv : materials) {
            materials.insert(kv);
        }
    }

};




class ObjLoader
{
public:
    
    // static methods
    //static MeshGroup *LoadMesh(std::string filename, bool loadMaterials = true);
    //static std::unordered_map<std::string, MeshMaterial *> loadMeshMaterial(std::string filename);
    
    //static MeshGroup *LoadMesh(LPSTR filename, bool loadMaterials = true);
    
    static Mesh *LoadSingleMesh(std::string filename, bool loadMaterials = true);
    
};

#endif /* obj_loader_hpp */
