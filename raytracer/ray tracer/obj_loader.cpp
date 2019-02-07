//
//  obj_loader.cpp
//  raytracer
//
//  Created by Adrian Russell on 26/12/2018.
//  Copyright © 2018 Adrian Russell. All rights reserved.
//

#include "obj_loader.hpp"


#include <sstream>
#include <fstream>
#include <iostream>


//#include "String_Additions.h"
//#include "ShaderManager.h"

std::string folderForPath(std::string& path)
{
    size_t found = path.find_last_of("/\\");
    return path.substr(0, found+1);
}

std::string filenameForPath(std::string& path)
{
    size_t found = path.find_last_of("/\\");
    return path.substr(found+2);
}

std::string TrimStart(std::string s)
{
    s.erase(s.begin(), std::find_if(s.begin(),
                                    s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

std::string TrimEnd(std::string s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(),
                         std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}






void loadFaces(std::string& line, std::vector<int>& vertexIndicies, std::vector<int>& normalIndicies, std::vector<int>& texUVIndicies) {
    std::istringstream sstream(line.substr(2));
    
    std::string tokenS;
    
    while (std::getline(sstream, tokenS, ' ')) {
        
        std::istringstream tokenstream(tokenS);
        
        int indexvertex = 0;
        int indextex = 0;
        int indexnormal = 0;
        
        std::string val;
        int cnt = 0;
        while (std::getline(tokenstream, val, '/')) {
            
            if (0 == cnt)
            {
                indexvertex = std::stoi(val);
            }
            else if (1 == cnt)
            {
                if (!val.empty()) {
                    indextex = std::stoi(val);
                }
            }
            else
            {
                if (!val.empty()) {
                    indexnormal = std::stoi(val);
                }
                
                break;
            }
            
            cnt++;
            
        }
        
        vertexIndicies.push_back(indexvertex);
        normalIndicies.push_back(indexnormal);
        texUVIndicies.push_back(indextex);
    }
}








std::unordered_map<std::string, MeshMaterial *> ObjLoader::loadMeshMaterial(std::string filename)
{
    std::string filePath = folderForPath(filename);
    
    std::ifstream          fileStream;
    std::string            line;
    
    std::unordered_map<std::string, MeshMaterial *> materials;
    
    MeshMaterial *mesh = NULL;
    
    fileStream.open(filename);
    //bool isOpen = fileStream.is_open();        //debugging only.
    
    
    while(std::getline(fileStream, line)) {
        
        // removing front whitespace
        line = TrimStart(line);
        
        //WCHAR oldStyleStr[200];
        //wcscpy(oldStyleStr, line.c_str());
        
        // if it's a comment (start with a #) the go to next line
        if (line[0] == '#') continue;
        
        if (line.substr(0, 6) == "newmtl") {
            std::string name = line.substr(7);
            
            mesh = new MeshMaterial();
            mesh->name = name;
            std::pair<std::string, MeshMaterial *> pair(name, mesh);
            materials.insert(pair);
        }
        
        
        if (line.substr(0,2) == "Ka") {
            float r,g,b;
            std::istringstream sstream(line.substr(3));
            sstream >> r >> g >> b;
            mesh->ambient = Color(r, b, g);
        }
        
        if (line.substr(0,2) == "Kd") {
            float r,g,b;
            std::istringstream sstream(line.substr(3));
            sstream >> r >> g >> b;
            mesh->diffuse = Color(r, b, g);
        }
        
        if (line.substr(0,2) =="Ks") {
            float r,g,b;
            std::istringstream sstream(line.substr(3));
            sstream >> r >> g >> b;
            mesh->specular = Color(r, b, g);
        }
        
        if (line.substr(0,5) == "Illum") {
            line = line.substr(6);
            int i;
            std::istringstream sstream(line);
            sstream >> i;
            mesh->illuminationModel = MeshMaterial::Illumination(i);
        }
        
        if (line.substr(0, 6) == "map_Kd") {
            mesh->diffuseTexture = line.substr(7);
        }
        
        if (line.substr(0, 6) == "map_Ka") {
            mesh->ambientTexture = line.substr(7);
        }
        
        if (line.substr(0, 6) == "map_Ks") {
            mesh->specularTexture = line.substr(7);
        }
        
        
    }
    return materials;
}


Mesh *ObjLoader::LoadSingleMesh(std::string filename, bool loadMaterials)
{
    std::string filePath = folderForPath(filename);
    
    Mesh *meshGroup = new Mesh();
    
    
    std::vector<Vector3> verticies;
    std::vector<Vector3> normals;
    std::vector<Vector3> TexUV;
    
    //std::vector<int> startOfGroup;
    //std::vector<int> startOfRange;
    std::vector<std::string> materialAtRange;
    
    std::vector<int> vertexIndicies;
    std::vector<int> normalIndicies;
    std::vector<int> texUVIndicies;
    
    std::ifstream          fileStream;
    std::string            line;
    
    
    fileStream.open(filename);
    //bool isOpen = fileStream.is_open();        //debugging only.
    
    int lineNumber       = 0;
    //int faceIndex = 0;
    while(std::getline(fileStream, line))
    {
        // trim leading whitespace
        line = TrimStart(line);
        
        //WCHAR oldStyleStr[200];
        //wcscpy(oldStyleStr, line.c_str());
        
        // if it's a comment (start with a #) the go to next line
        if (line[0] == '#') continue;
        
        if (line.substr(0, 6) == "mtllib") {
            
            continue;
            //if (loadMaterials) {
            //    int length = line.length();
            //    std::wstring wname = line.substr(7, length - 7);
            
            //    std::string name(wname.begin(), wname.end());
            
            //    std::string materialPath = filePath + name;
            
            //    LPSTR materialPath2 = const_cast<char *>(materialPath.c_str());
            
            //    std::unordered_map<std::wstring, MeshMaterial *> newMaterials = loadMeshMaterial(materialPath2);
            
            //    meshGroup->addMaterials(newMaterials);
            //}
        }
        
        //******************************************************************//
        // If true, we have found a vertex.  Read it in as a 2 character    //
        // string, followed by 3 decimal numbers.    Suprisingly the C++        //
        // string has no split() method.   I am using really old stuff,        //
        // fscanf.  There  must be a better way, use regular expressions?    //
        //******************************************************************//
        if (line.substr(0, 2) == "v ") {
            float x, y, z;
            std::istringstream sstream(line.substr(2));
            sstream >> x >> y >> z;
            Vector3 v(x, y, z);
            verticies.push_back(v);
        }
        
        
        if (line.substr(0, 2) == "vn") {
            float x, y, z;
            std::istringstream sstream(line.substr(3));
            sstream >> x >> y >> z;
            Vector3 v(x, y, z);
            normals.push_back(v);
        }
        
        
        if (line.substr(0, 2) == "vt") {
            float x, y;
            std::istringstream sstream(line.substr(3));
            sstream >> x >> y;
            Vector3 v(x, y, 1);
            TexUV.push_back(v);
        }
        
        //******************************************************************//
        // If true, we have found a face.   Read it in as a 2 character        //
        // string, followed by 3 decimal numbers.    Suprisingly the C++        //
        // string has no split() method.   I am using really old stuff,        //
        // fscanf.  There must be a better way, use regular expressions?    //
        //                                                                    //
        // It assumes the line is in the format                                //
        // f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3 ...                            //
        //******************************************************************//
        if (line[0] == 'f') {
            
            std::istringstream sstream(line.substr(2));
            
            std::string tokenS;
            
            int vertexCount = 0;
            int fiv = 0;
            int fit = 0;
            int fin = 0;
            while (std::getline(sstream, tokenS, ' ')) {
                
                std::istringstream tokenstream(tokenS);
                
                int indexvertex = 0;
                int indextex    = 0;
                int indexnormal = 0;
                
                std::string val;
                int cnt = 0;
                while (std::getline(tokenstream, val, '/')) {
                    
                    if (0 == cnt) {
                        indexvertex = std::stoi(val);
                    } else if (1 == cnt) {
                        if (!val.empty()) {
                            indextex = std::stoi(val);
                        }
                    } else {
                        if (!val.empty()) {
                            indexnormal = std::stoi(val);
                        }
                        
                        break;
                    }
                    
                    cnt++;
                    
                }
                
                if (vertexCount == 0) {
                    fiv = indexvertex;
                    fin = indexnormal;
                    fit = indextex;
                }
                
                if (vertexCount >= 3) {
                    vertexIndicies.push_back(fiv);
                    normalIndicies.push_back(fin);
                    texUVIndicies.push_back(fit);
                    
                    vertexIndicies.push_back(vertexIndicies[vertexIndicies.size() - 2]);
                    normalIndicies.push_back(normalIndicies[normalIndicies.size() - 2]);
                    texUVIndicies.push_back(texUVIndicies[texUVIndicies.size() - 2]);
                }
                
                vertexIndicies.push_back(indexvertex);
                normalIndicies.push_back(indexnormal);
                texUVIndicies.push_back(indextex);
                
                vertexCount++;
            }
            
            //faceIndex += 3;
        }
        
        
        
        if (line.substr(0, 6) == "usemtl") {
            continue;
            /*int length = line.length();
             std::wstring name = line.substr(7, length - 7);
             materialAtRange.push_back(name);
             startOfRange.push_back(faceIndex);*/
        }
        
        lineNumber++;
    }
    
    
    std::vector<SimpleVertex> vertices;
    std::vector<int> indices;
    
    int index = 0;
    
    bool requiresCalculatingNormals = false;
    
    ///////////////////////// END SCANNING FILE
    for (unsigned int i = 0; i < vertexIndicies.size(); i++)
    {
        SimpleVertex sv;
        sv.Pos = verticies[vertexIndicies[i]-1];
        
        if (normalIndicies[i] > 0 && normals.size() > normalIndicies[i]-1) {
            sv.VecNormal = normals[normalIndicies[i]-1];
        } else {
            requiresCalculatingNormals = true;
        }
        
        
        if (texUVIndicies[i] > 0 && TexUV.size() > texUVIndicies[i]-1) {
            sv.TexUV.x = TexUV[texUVIndicies[i]-1].x;
            sv.TexUV.y = TexUV[texUVIndicies[i]-1].y;
        }
        
        vertices.push_back(sv);
        
        indices.push_back(index);
        index++;
    }
    
    if (requiresCalculatingNormals) {
        std::vector<std::vector<Vector3>> normalBuffers(vertices.size());
        
        for (unsigned int i = 0; i < indices.size(); i+=3) {
            int x = indices[i];
            int y = indices[i+1];
            int z = indices[i+2];
            
            Vector3 v1 = vertices[y].Pos - vertices[x].Pos;
            Vector3 v2 = vertices[z].Pos - vertices[x].Pos;
            Vector3 n = Vector3::crossProduct(v1, v2);
            
            normalBuffers[x].push_back(n);
            normalBuffers[y].push_back(n);
            normalBuffers[z].push_back(n);
        }
        
        for (unsigned int i = 0; i < vertices.size(); i++) {
            Vector3 n;
            for (unsigned int j = 0; j < normalBuffers[i].size(); j++) {
                n += normalBuffers[i][j];
            }
            n = n.normalise();
            vertices[i].VecNormal = n;
        }
    }
    
    
    meshGroup->vertices = vertices;
    meshGroup->indices = indices;
    //meshGroup->startOfRange = startOfRange;
    //meshGroup->startOfGroup = startOfGroup;
    //meshGroup->materialAtRange = materialAtRange;
    
    
    return meshGroup;
}


MeshGroup *ObjLoader::LoadMesh(std::string filename, bool loadMaterials)
{
    std::string filePath = folderForPath(filename);
    
    MeshGroup *meshGroup = new MeshGroup();
    
    
    std::vector<Vector3> verticies;
    std::vector<Vector3> normals;
    std::vector<Vector3> TexUV;
    
    std::vector<int> startOfGroup;
    std::vector<int> startOfRange;
    std::vector<std::string> materialAtRange;
    
    std::vector<int> vertexIndicies;
    std::vector<int> normalIndicies;
    std::vector<int> texUVIndicies;
    
    std::ifstream          fileStream;
    std::string            line;
    
    
    fileStream.open(filename);
    //bool isOpen = fileStream.is_open();        //debugging only.
    
    int lineNumber       = 0;
    int faceIndex = 0;
    while(std::getline(fileStream, line))
    {
        // trim leading whitespace
        line = TrimStart(line);
        
       // WCHAR oldStyleStr[200];
       // wcscpy(oldStyleStr, line.c_str());
        
        // if it's a comment (start with a #) the go to next line
        if (line[0] == '#') continue;
        
        if (line[0] == 'g') {
            startOfGroup.push_back(faceIndex);
        }
        
        if (line.substr(0, 6) == "mtllib") {
            
            if (loadMaterials) {
                std::string name = line.substr(7);
                
                //std::string name(wname.begin(), wname.end());
                
                std::string materialPath = filePath + name;
                
                //LPSTR materialPath2 = const_cast<char *>(materialPath.c_str());
                
                std::unordered_map<std::string, MeshMaterial *> newMaterials = loadMeshMaterial(materialPath);
                
                meshGroup->addMaterials(newMaterials);
            }
        }
        
        //******************************************************************//
        // If true, we have found a vertex.  Read it in as a 2 character    //
        // string, followed by 3 decimal numbers.    Suprisingly the C++        //
        // string has no split() method.   I am using really old stuff,        //
        // fscanf.  There  must be a better way, use regular expressions?    //
        //******************************************************************//
        if (line[0] == 'v') {
            float x, y, z;
            std::istringstream sstream(line.substr(2));
            sstream >> x >> y >> z;
            Vector3 v(x, y, z);
            verticies.push_back(v);
        }
        
        
        if (line.substr(0, 2) == "vn") {
            float x, y, z;
            std::istringstream sstream(line.substr(3));
            sstream >> x >> y >> z;
            Vector3 v(x, y, z);
            normals.push_back(v);
        }
        
        
        if (line.substr(0, 2) == "vt") {
            float x, y;
            std::istringstream sstream(line.substr(3));
            sstream >> x >> y;
            Vector3 v(x, y, 1);
            TexUV.push_back(v);
        }
        
        //******************************************************************//
        // If true, we have found a face.   Read it in as a 2 character        //
        // string, followed by 3 decimal numbers.    Suprisingly the C++        //
        // string has no split() method.   I am using really old stuff,        //
        // fscanf.  There must be a better way, use regular expressions?    //
        //                                                                    //
        // It assumes the line is in the format                                //
        // f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3 ...                            //
        //******************************************************************//
        if (line[0] == 'f') {
            loadFaces(line, vertexIndicies, normalIndicies, texUVIndicies);
            
            faceIndex += 3;
        }
        
        if (line.substr(0, 6) == "usemtl") {
            std::string name = line.substr(7);
            
            materialAtRange.push_back(name);
            startOfRange.push_back(faceIndex);
        }
        
        lineNumber++;
    }
    
    
    std::vector<SimpleVertex> vertices;
    std::vector<int> indices;
    
    int index = 0;
    
    ///////////////////////// END SCANNING FILE
    for (unsigned int i = 0; i < vertexIndicies.size(); i++)
    {
        
        
        SimpleVertex sv;
        sv.Pos.x = verticies[vertexIndicies[i]-1].x;
        sv.Pos.y = verticies[vertexIndicies[i]-1].y;
        sv.Pos.z = verticies[vertexIndicies[i]-1].z;
        
        sv.VecNormal.x = normals[normalIndicies[i]-1].x;
        sv.VecNormal.y = normals[normalIndicies[i]-1].y;
        sv.VecNormal.z = normals[normalIndicies[i]-1].z;
        
        sv.TexUV.x = TexUV[texUVIndicies[i]-1].x;
        sv.TexUV.y = TexUV[texUVIndicies[i]-1].y;
        
        vertices.push_back(sv);
        
        indices.push_back(index);
        index++;
    }
    
    meshGroup->vertices = vertices;
    meshGroup->indices = indices;
    meshGroup->startOfRange = startOfRange;
    meshGroup->startOfGroup = startOfGroup;
    meshGroup->materialAtRange = materialAtRange;
    
    
    return meshGroup;
}






