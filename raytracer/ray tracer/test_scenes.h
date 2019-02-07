//
//  test_scenes.h
//  raytracer
//
//  Created by Adrian Russell on 21/12/2018.
//  Copyright © 2018 Adrian Russell. All rights reserved.
//

#ifndef test_scenes_h
#define test_scenes_h

#include "scene.h"
#include "Vector3.hpp"
#include "Ray.hpp"
#include "Material.hpp"
#include "hitable.hpp"
#include "hitable_list.hpp"
#include "sphere.hpp"
#include "camera.hpp"
#include "lambertian.hpp"
#include "metal.hpp"
#include "dielectric.hpp"
#include "texture.hpp"
#include "perlin.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "CoreFoundation/CoreFoundation.h"
#include "image_texture.hpp"
#include "diffuse_light.hpp"
#include "xy_rect.hpp"
#include "box.hpp"
#include "translate.hpp"
#include "bvh.hpp"
#include "triangle.hpp"
#include "obj_loader.hpp"
#include "anisotropic_phong.hpp"


image_texture *textureFromFilename(CFStringRef filename) {
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef resourcesURL = CFBundleCopyResourceURL(mainBundle, filename, NULL, NULL);
    char path[PATH_MAX];
    CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, PATH_MAX);
    CFRelease(resourcesURL);
    int nx, ny, nn;
    unsigned char *tex_data = stbi_load(path, &nx, &ny, &nn, 0);
    return new image_texture(tex_data, nx, ny);
}

Mesh *meshFromFilename(CFStringRef filename) {
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef resourcesURL = CFBundleCopyResourceURL(mainBundle, filename, NULL, NULL);
    char path[PATH_MAX];
    if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, PATH_MAX)) {
        // error!
    }
    CFRelease(resourcesURL);
    return ObjLoader::LoadSingleMesh(path);
}


scene *random_scene() {
    //texture *checkerTexture = new checker_texture(new constant_texture(Color(0.2,0.3,0.1)), new constant_texture(Color(0.9,0.9,0.9)));
    //texture *noiseTexture = new noise_texture(10);

    int n = 500;
    hitable **list = new hitable*[n + 1];
    list[0] = new sphere(Vector3(0,-1000,-0), 1000,new lambertian(new constant_texture(Color(0.5, 0.5, 0.5))));
    int i = 1;

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            float choose_mat = drand48();
            Vector3 center(a+0.9*drand48(), 0.2, b+0.9*drand48());
            if ((center-Vector3(4,0.2,0)).length() > 0.9) {
                if (choose_mat < 0.8) {
                    // diffuse
                    texture *tex = new constant_texture(Color(drand48()*drand48(), drand48()*drand48(), drand48()*drand48()));
                    list[i++] = new sphere(center, 0.2, new lambertian(tex));
                } else if (choose_mat < 0.95) {
                    // metal
                    list[i++] = new sphere(center, 0.2, new metal(Color(0.5*(1+drand48()), 0.5*(1+drand48()), 0.5*(1+drand48())), 0.01));
                } else {
                    // glass
                    list[i++] = new sphere(center, 0.2, new dielectric(1.5));
                }
            }
        }
    }


    //texture *colorTexture = new constant_texture(Color(0.4,0.2,0.1));
    image_texture *earthTexture  = textureFromFilename(CFSTR("1_earth.jpg"));

    list[i++] = new sphere(Vector3(0,1,0), 1.0, new dielectric(1.5));
    list[i++] = new sphere(Vector3(-4,1,0), 1.0, new lambertian(earthTexture));
    list[i++] = new sphere(Vector3(4,1,0), 1.0, new metal(Color(0.7, 0.6, 0.5), 0.01));

    hitable *world = new hitable_list(list, i);

    //bvh_node *world = new bvh_node(list, i, 0.0, 1.0);

    Vector3 lookfrom(13,2,3);
    Vector3 lookat(0,0,0);
    float dist_to_focus = 10.0;
    float aperture = 0.1;
    float aspectRatio = 3.0/2.0;
    camera *cam = new cameraC(lookfrom, lookat, Vector3(0,1,0), 20, aspectRatio, aperture, dist_to_focus, 0.0, 1.0);

    skybox *sky_box = new sky_skybox();
    //skybox *sky_box = new constant_skybox(Color(0.5,0.7,1));

     return new scene(world, list[i-3], cam, sky_box, aspectRatio);
}


scene *cornellBoxWithSuzanne() {
    material *blue  = new lambertian( new constant_texture(Color(0.05, 0.05, 0.75)) );
    material *red   = new lambertian( new constant_texture(Color(0.65, 0.05, 0.05)) );
    material *white = new lambertian( new constant_texture(Color(0.73, 0.73, 0.73)) );
    material *green = new lambertian( new constant_texture(Color(0.12, 0.45, 0.15)) );
    material *light = new diffuse_light( new constant_texture(Color(15, 15, 15)) );
    material *mirror = new metal(Color(1.0,1.0,1.0), 0.0);
    dielectric *glass = new dielectric(1.5);
    glass->density = 0.03;
    glass->volumeColor = Color(1.0,1.0,0.0);
    
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef resourcesURL = CFBundleCopyResourceURL(mainBundle, CFSTR("suzanne.obj"), NULL, NULL);
    char path[PATH_MAX];
    if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, PATH_MAX)){
        // error!
    }
    CFRelease(resourcesURL);
    
    auto suzanneMesh = ObjLoader::LoadSingleMesh(path);
    hitable *suzanne = suzanneMesh->create_hitable(blue);
    hitable *suzanneFinal = new translate(new rotate_y(new scale(suzanne, 160), 180), Vector3(267.5, 277.5, 860));
//    aabb hit;
//    suzanneFinal->bounding_box(0, 1, hit);
    hitable **list = new hitable*[10];
    int i = 0;
    list[i++] = new flip_normals(new yz_rect(0,555,0,555,555, green));
    list[i++] = new yz_rect(0,555,0,555,0, red);
    //list[i++] = new yz_rect(100,455,100,455,0, mirror);
    list[i++] = new flip_normals(new xz_rect(213,343,227,332,554, light));
    list[i++] = new flip_normals(new xz_rect(0,555,0,555,555, white));
    list[i++] = new xz_rect(0,555,0,555,0, white);
    list[i++] = new flip_normals(new xy_rect(0,555,0,555,555, white));
    //list[i++] = new translate(new rotate_y(new box(Vector3(0,0,0), Vector3(165,330,165), aluminium), 15), Vector3(265,0,295));
    //list[i++] = new sphere(Vector3(190, 90, 190), 90, glass);
    list[i++] = suzanneFinal;
    hitable *world = new hitable_list(list,i);
    
    
    hitable *light_shape = new xz_rect(213, 343, 227, 332, 554, nullptr);
    //hitable *glass_sphere = new sphere(Vector3(190, 90, 190), 90, nullptr);
    hitable **a = new hitable*[2];
    a[0] = light_shape;
    a[1] = suzanneFinal;
    hitable_list *hlist = new hitable_list(a,2);
    
    
    Vector3 lookfrom(278,278,-800);
    Vector3 lookat(278,278,0);
    float dist_to_focus = 10.0;
    float aperture = 0.0;
    float vfov = 40.0;
    float aspectRatio = 1.0;
    camera *cam = new cameraC(lookfrom, lookat, Vector3(0,1,0), vfov, aspectRatio, aperture, dist_to_focus, 0.0, 1.0);
    
    skybox *sky_box = new constant_skybox();
    
    return new scene(world, hlist, cam, sky_box, aspectRatio);
}


scene* cornellBox() {
    material *red   = new lambertian( new constant_texture(Color(0.65, 0.05, 0.05)) );
    material *white = new lambertian( new constant_texture(Color(0.73, 0.73, 0.73)) );
    material *green = new lambertian( new constant_texture(Color(0.12, 0.45, 0.15)) );
    material *light = new diffuse_light( new constant_texture(Color(15, 15, 15)) );
    hitable **list = new hitable*[8];
    int i = 0;
    list[i++] = new flip_normals(new yz_rect(0,555,0,555,555, green));
    list[i++] = new yz_rect(0,555,0,555,0, red);
    list[i++] = new flip_normals(new xz_rect(213,343,227,332,554, light));
    list[i++] = new flip_normals(new xz_rect(0,555,0,555,555, white));
    list[i++] = new xz_rect(0,555,0,555,0, white);
    list[i++] = new flip_normals(new xy_rect(0,555,0,555,555, white));
    list[i++] = new translate(new rotate_y(new box(Vector3(0,0,0), Vector3(165,165,165), white), -18), Vector3(130, 0, 65));
    list[i++] = new translate(new rotate_y(new box(Vector3(0,0,0), Vector3(165,330,165), white), 15), Vector3(265,0,295));
    hitable *world = new hitable_list(list,i);
    
    
    hitable *light_shape = new xz_rect(213, 343, 227, 332, 554, nullptr);
    
    
    Vector3 lookfrom(278,278,-800);
    Vector3 lookat(278,278,0);
    float dist_to_focus = 10.0;
    float aperture = 0.0;
    float vfov = 40.0;
    float aspectRatio = 1.0;
    camera *cam = new cameraC(lookfrom, lookat, Vector3(0,1,0), vfov, aspectRatio, aperture, dist_to_focus, 0.0, 1.0);
    
    skybox *sky_box = new constant_skybox();
    
    return new scene(world, light_shape, cam, sky_box, aspectRatio);
}

scene* cornellBoxWithCheckers() {
    material *red   = new lambertian( new constant_texture(Color(0.65, 0.05, 0.05)) );
    material *white = new lambertian( new constant_texture(Color(0.73, 0.73, 0.73)) );
    material *green = new lambertian( new constant_texture(Color(0.12, 0.45, 0.15)) );
    material *light = new diffuse_light( new constant_texture(Color(15, 15, 15)) );
    material *checkers = new lambertian(new checker_texture(new constant_texture(Color(0.1, 0.1, 0.1)), new constant_texture(Color(0.73, 0.73, 0.73))));
    dielectric *glass = new dielectric(1.5);
//    glass->density = 0.03;
//    glass->volumeColor = Color(1.0,1.0,0.0);
    
    hitable **list = new hitable*[8];
    int i = 0;
    list[i++] = new flip_normals(new yz_rect(0,555,0,555,555, green));
    list[i++] = new yz_rect(0,555,0,555,0, red);
    list[i++] = new flip_normals(new xz_rect(213,343,227,332,554, light));
    list[i++] = new flip_normals(new xz_rect(0,555,0,555,555, white));
    list[i++] = new xz_rect(0,555,0,555,0, white);
    list[i++] = new flip_normals(new xy_rect(0,555,0,555,555, checkers));  // back wall
    //list[i++] = new translate(new rotate_y(new box(Vector3(0,0,0), Vector3(165,165,165), white), -18), Vector3(130, 0, 65));
    list[i++] = new translate(new rotate_y(new box(Vector3(0,0,0), Vector3(165,330,165), white), 15), Vector3(265,0,295));
    list[i++] = new sphere(Vector3(190, 300, 190), 90, glass);
    hitable *world = new hitable_list(list,i);
    
    
    hitable *light_shape = new xz_rect(213, 343, 227, 332, 554, nullptr);
    
    
    Vector3 lookfrom(278,278,-800);
    Vector3 lookat(278,278,0);
    float dist_to_focus = 10.0;
    float aperture = 0.0;
    float vfov = 40.0;
    float aspectRatio = 1.0;
    camera *cam = new cameraC(lookfrom, lookat, Vector3(0,1,0), vfov, aspectRatio, aperture, dist_to_focus, 0.0, 1.0);
    
    skybox *sky_box = new constant_skybox();
    
    return new scene(world, light_shape, cam, sky_box, aspectRatio);
}


scene* cornellBoxWithTriangle() {
    material *blue  = new lambertian( new constant_texture(Color(0.05, 0.05, 0.75)) );
    material *red   = new lambertian( new constant_texture(Color(0.65, 0.05, 0.05)) );
    material *white = new lambertian( new constant_texture(Color(0.73, 0.73, 0.73)) );
    material *green = new lambertian( new constant_texture(Color(0.12, 0.45, 0.15)) );
    material *light = new diffuse_light( new constant_texture(Color(15, 15, 15)) );
    hitable **list = new hitable*[12];
    int i = 0;
    list[i++] = new flip_normals(new yz_rect(0,555,0,555,555, green));
    list[i++] = new yz_rect(0,555,0,555,0, red);
    list[i++] = new flip_normals(new xz_rect(213,343,227,332,554, light));
    list[i++] = new flip_normals(new xz_rect(0,555,0,555,555, white));
    
    //list[i++] = new flip_normals(new xz_rect(0,555,0,555,420, glass));
    
    list[i++] = new xz_rect(0,555,0,555,0, white);
    list[i++] = new flip_normals(new xy_rect(0,555,0,555,555, white));
    list[i++] = new translate(new rotate_y(new box(Vector3(0,0,0), Vector3(165,165,165), white), -18), Vector3(130, 0, 65));
    list[i++] = new translate(new rotate_y(new box(Vector3(0,0,0), Vector3(165,330,165), white), 15), Vector3(265,0,295));
    //list[i++] = new translate(new triangle(Vector3(170, 120, 170), Vector3(340, 140, 180), Vector3(190, 200, 330), red), Vector3(0,0,300));
    list[i++] = new triangle(Vector3(170, 220, 170), Vector3(190, 250, 260), Vector3(240, 240, 180), blue);
    hitable *world = new hitable_list(list,i);
    
    
    hitable *light_shape = new xz_rect(213, 343, 227, 332, 554, nullptr);
    
    
    Vector3 lookfrom(278,278,-800);
    Vector3 lookat(278,278,0);
    float dist_to_focus = 10.0;
    float aperture = 0.0;
    float vfov = 40.0;
    float aspectRatio = 1.0;
    camera *cam = new cameraC(lookfrom, lookat, Vector3(0,1,0), vfov, aspectRatio, aperture, dist_to_focus, 0.0, 1.0);
    
    skybox *sky_box = new constant_skybox();
    
    return new scene(world, light_shape, cam, sky_box, aspectRatio);
}

scene* cornellBoxWithSphere() {
    material *red   = new lambertian( new constant_texture(Color(0.65, 0.05, 0.05)) );
    material *white = new lambertian( new constant_texture(Color(0.73, 0.73, 0.73)) );
    material *green = new lambertian( new constant_texture(Color(0.12, 0.45, 0.15)) );
    material *light = new diffuse_light( new constant_texture(Color(15, 15, 15)) );
    material *aluminium = new metal(Color(0.8,0.85,0.88), 0.0);
    dielectric *glass = new dielectric(1.5);
    glass->density = 0.03;
    glass->volumeColor = Color(0.0,0.0,0.0);
    hitable **list = new hitable*[8];
    int i = 0;
    list[i++] = new flip_normals(new yz_rect(0,555,0,555,555, green));
    list[i++] = new yz_rect(0,555,0,555,0, red);
    list[i++] = new flip_normals(new xz_rect(213,343,227,332,554, light));
    list[i++] = new flip_normals(new xz_rect(0,555,0,555,555, white));
    list[i++] = new xz_rect(0,555,0,555,0, white);
    list[i++] = new flip_normals(new xy_rect(0,555,0,555,555, white));
    list[i++] = new translate(new rotate_y(new box(Vector3(0,0,0), Vector3(165,330,165), aluminium), 15), Vector3(265,0,295));
    list[i++] = new sphere(Vector3(190, 90, 190), 90, glass);
    hitable *world = new hitable_list(list,i);
    
    
    hitable *light_shape = new xz_rect(213, 343, 227, 332, 554, nullptr);
    hitable *glass_sphere = new sphere(Vector3(190, 90, 190), 90, nullptr);
    hitable **a = new hitable*[2];
    a[0] = light_shape;
    a[1] = glass_sphere;
    hitable_list *hlist = new hitable_list(a,2);
    
    
    Vector3 lookfrom(278,278,-800);
    Vector3 lookat(278,278,0);
    float dist_to_focus = 10.0;
    float aperture = 0.0;
    float vfov = 40.0;
    float aspectRatio = 1.0;
    camera *cam = new cameraC(lookfrom, lookat, Vector3(0,1,0), vfov, aspectRatio, aperture, dist_to_focus, 0.0, 1.0);
    
    skybox *sky_box = new constant_skybox();
    
    return new scene(world, hlist, cam, sky_box, aspectRatio);
}

scene* cornellBoxWithExtraSpheres() {
    return nullptr;
}

scene* rbgLightSpheres() {
    material *white = new lambertian( new constant_texture(Color(0.73, 0.73, 0.73)) );
    material *lightR = new diffuse_light( new constant_texture(Color(20, 0, 0)) );
    material *lightG = new diffuse_light( new constant_texture(Color(0, 20, 0)) );
    material *lightB = new diffuse_light( new constant_texture(Color(0, 0, 20)) );
    material *glass = new dielectric(1.8);
    hitable **list = new hitable*[8];
    int i = 0;
    list[i++] = new sphere(Vector3(10,10,10), 10, glass);
    list[i++] = new sphere(Vector3(30,10,10), 10, glass);
    list[i++] = new sphere(Vector3(50,10,10), 10, glass);
    list[i++] = new flip_normals(new xz_rect(24, 30, 5, 25, 40, lightR));
    list[i++] = new flip_normals(new xz_rect(32, 38, 5, 25, 40, lightG));
    list[i++] = new flip_normals(new xz_rect(40, 46, 5, 25, 40, lightB));
    list[i++] = new xz_rect(-1000, 1000, -1000, 1000, 0, white);
    //list[i++] = new flip_normals(new xz_rect(-10, 70, -100, 100, 80, white));
    hitable *world = new hitable_list(list,i);
    
    
    
    Vector3 lookfrom(30,40,-60);
    Vector3 lookat(30,0,20);
    float dist_to_focus = 50.0;
    float aperture = 0.0;
    float vfov = 35.0;
    float aspectRatio = 2.0;
    camera *cam = new cameraC(lookfrom, lookat, Vector3(0,1,0), vfov, aspectRatio, aperture, dist_to_focus, 0.0, 1.0);
    
    
    hitable **a = new hitable*[6];
    a[0] = new sphere(Vector3(10,10,10), 10, nullptr);
    a[1] = new sphere(Vector3(30,10,10), 10, nullptr);
    a[2] = new sphere(Vector3(50,10,10), 10, nullptr);
    a[3] = new xz_rect(24, 30, 5,  25, 40, nullptr); // 45, 43  - 2
    a[4] = new xz_rect(32, 38, 5,  25, 40, nullptr); // 32, 38  - 6
    a[5] = new xz_rect(40, 46, 5,  25, 40, nullptr); // 25, 28  - 7
    hitable_list *hlist = new hitable_list(a,6);
    
    skybox *sky_box = new constant_skybox();
    
    return new scene(world, hlist, cam, sky_box, aspectRatio);
}


scene* cubemapSkyboxScene() {
    image_texture *earthTexture  = textureFromFilename(CFSTR("1_earth.jpg"));
    material *earthMaterial = new lambertian(earthTexture);
    material *mirror = new metal(Color(1.0,1.0,1.0), 0.0);
    material *glass = new dielectric(1.8);
    
    // create the skybox
    image_texture *front  = textureFromFilename(CFSTR("yokohama3-front.jpg"));
    image_texture *back   = textureFromFilename(CFSTR("yokohama3-back.jpg"));
    image_texture *left   = textureFromFilename(CFSTR("yokohama3-left.jpg"));
    image_texture *right  = textureFromFilename(CFSTR("yokohama3-right.jpg"));
    image_texture *top    = textureFromFilename(CFSTR("yokohama3-top.jpg"));
    image_texture *bottom = textureFromFilename(CFSTR("yokohama3-bottom.jpg"));
    skybox *sky_box = new cubemap_skybox(bottom, top, left, right, front, back);
    
    
    Vector3 lookfrom(20,0,-60);
    Vector3 lookat(0,0,0);
    float dist_to_focus = 50.0;
    float aperture = 0.0;
    float vfov = 35.0;
    float aspectRatio = 1.0;
    camera *cam = new cameraC(lookfrom, lookat, Vector3(0,1,0), vfov, aspectRatio, aperture, dist_to_focus, 0.0, 1.0);
    
    
    hitable *s = new sphere(Vector3(0,0,0), 15, glass);
    
    return new scene(s, nullptr, cam, sky_box, aspectRatio);
}

scene *phongSpheresScene() {
    texture *blue   = new constant_texture(Color(0.05, 0.05, 0.75));
    texture *red    = new constant_texture(Color(0.65, 0.05, 0.05));
    texture *white  = new constant_texture(Color(0.73, 0.73, 0.73));
    texture *green  = new constant_texture(Color(0.12, 0.45, 0.15));
    texture *yellow = new constant_texture(Color(0.75, 0.75, 0.05));
    texture *specular = new constant_texture(Color(1.0, 1.0, 1.0));
    
    material *lightMaterial = new diffuse_light( new constant_texture(Color(15, 15, 15)) );
    hitable *light = new flip_normals(new xz_rect(-10,10,-10,10,80, lightMaterial));
    
    double nu = 100;
    double nv = 200;
    
    material *bluePhong   = new anisotropic_phong(blue,   specular, nu, nv);
    material *redPhong    = new anisotropic_phong(red,    specular, nu, nv);
    material *yellowPhong = new anisotropic_phong(yellow, specular, nu, nv);
    material *greenPhong  = new anisotropic_phong(green,  specular, nu, nv);
    material *whitePhong  = new anisotropic_phong(white,  specular, nu, nv);
    
    hitable **list = new hitable*[8];
    int i = 0;
    list[i++] = new xz_rect(-100,100,-100,100, 0, whitePhong);
    list[i++] = new sphere(Vector3(20, 10, 0), 10, bluePhong);
    list[i++] = new sphere(Vector3(-20,10, 0), 10, redPhong);
    list[i++] = new sphere(Vector3(0,  10, 20), 10, yellowPhong);
    list[i++] = new sphere(Vector3(0,  10, -20), 10, greenPhong);
    list[i++] = light;
    hitable *world = new hitable_list(list,i);
    
    skybox *sky_box = new constant_skybox();
    
    Vector3 lookfrom(0,80,-60);
    Vector3 lookat(0,0,0);
    float dist_to_focus = 50.0;
    float aperture = 0.0;
    float vfov = 35.0;
    float aspectRatio = 1.0;
    camera *cam = new cameraC(lookfrom, lookat, Vector3(0,1,0), vfov, aspectRatio, aperture, dist_to_focus, 0.0, 1.0);
    
    return new scene(world, light, cam, sky_box, aspectRatio);
}

scene *legoBricks() {
    Mesh *brickMesh = meshFromFilename(CFSTR("lego_brick.obj"));
    
    material *blue  = new lambertian( new constant_texture(Color(0.05, 0.05, 0.75)) );
    material *red   = new lambertian( new constant_texture(Color(0.65, 0.05, 0.05)) );
    material *white = new lambertian( new constant_texture(Color(0.73, 0.73, 0.73)) );
    material *green = new lambertian( new constant_texture(Color(0.12, 0.45, 0.15)) );
    material *light = new diffuse_light( new constant_texture(Color(15, 15, 15)) );
    
    
    hitable **list = new hitable*[9];
    int i = 0;
    list[i++] = new flip_normals(new yz_rect(0,555,0,555,555, green));
    list[i++] = new yz_rect(0,555,0,555,0, red);
    list[i++] = new flip_normals(new xz_rect(200,356,214,345,554, light));
    list[i++] = new flip_normals(new xz_rect(0,555,0,555,555, white));
    list[i++] = new xz_rect(0,555,0,555,0, white);
    list[i++] = new flip_normals(new xy_rect(0,555,0,555,555, white));
    //list[i++] = new translate(new rotate_y(new box(Vector3(0,0,0), Vector3(165,165,165), white), -18), Vector3(130, 0, 65));
    //list[i++] = new translate(new rotate_y(new box(Vector3(0,0,0), Vector3(165,330,165), white), 15), Vector3(265,0,295));
    
    hitable *brick = brickMesh->create_hitable(blue);
    hitable *brickFinal = new translate(new rotate_x(new scale(brick, 100.0), 270), Vector3(267.5, 0, 300));
    list[i++] = brickFinal;
    
    hitable *world = new hitable_list(list,i);
    
    
    hitable *light_shape = new xz_rect(200,356,214,345, 554, nullptr);
    
    
    Vector3 lookfrom(278,278,-800);
    Vector3 lookat(278,278,0);
    float dist_to_focus = 10.0;
    float aperture = 0.0;
    float vfov = 40.0;
    float aspectRatio = 1.0;
    camera *cam = new cameraC(lookfrom, lookat, Vector3(0,1,0), vfov, aspectRatio, aperture, dist_to_focus, 0.0, 1.0);
    
    skybox *sky_box = new constant_skybox();
    
    return new scene(world, light_shape, cam, sky_box, aspectRatio);
    
    
}

scene *legoMan() {
    Mesh *brickMesh = meshFromFilename(CFSTR("lego_man.obj"));
    
    material *blue  = new lambertian( new constant_texture(Color(0.05, 0.05, 0.85)) );
    material *red   = new lambertian( new constant_texture(Color(0.65, 0.05, 0.05)) );
    material *white = new lambertian( new constant_texture(Color(0.73, 0.73, 0.73)) );
    material *green = new lambertian( new constant_texture(Color(0.12, 0.45, 0.15)) );
    material *light = new diffuse_light( new constant_texture(Color(15, 15, 15)) );
    
    
    hitable **list = new hitable*[9];
    int i = 0;
    list[i++] = new flip_normals(new yz_rect(0,555,0,555,555, green));
    list[i++] = new yz_rect(0,555,0,555,0, red);
    list[i++] = new flip_normals(new xz_rect(200,356,214,345,554, light));
    //list[i++] = new xy_rect(200,356,214,345,-800, light);
    list[i++] = new flip_normals(new xz_rect(0,555,0,555,555, white));
    list[i++] = new xz_rect(0,555,0,555,0, white);
    list[i++] = new flip_normals(new xy_rect(0,555,0,555,555, white));
    //list[i++] = new translate(new rotate_y(new box(Vector3(0,0,0), Vector3(165,165,165), white), -18), Vector3(130, 0, 65));
    //list[i++] = new translate(new rotate_y(new box(Vector3(0,0,0), Vector3(165,330,165), white), 15), Vector3(265,0,295));
    
    hitable *brick = brickMesh->create_hitable(blue);
    hitable *brickFinal = new translate(new rotate_y(new scale(brick, 60.0), 45.0), Vector3(267.5, 00, 300));
    list[i++] = brickFinal;
    
    hitable *world = new hitable_list(list,i);
    
    hitable **lights = new hitable*[2];
    lights[0] = new xz_rect(200,356,214,345, 554, nullptr);
    hitable *light_list = new hitable_list(lights, 1);
    
    Vector3 lookfrom(278,278,-800);
    Vector3 lookat(278,278,0);
    float dist_to_focus = 10.0;
    float aperture = 0.0;
    float vfov = 40.0;
    float aspectRatio = 1.0;
    camera *cam = new cameraC(lookfrom, lookat, Vector3(0,1,0), vfov, aspectRatio, aperture, dist_to_focus, 0.0, 1.0);
    
    skybox *sky_box = new constant_skybox();
    
    return new scene(world, light_list, cam, sky_box, aspectRatio);
    
    
}


#endif /* test_scenes_h */
