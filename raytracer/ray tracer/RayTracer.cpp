//
//  RayTracer.cpp
//  raytracer
//
//  Created by Adrian Russell on 29/06/2018.
//  Copyright © 2018 Adrian Russell. All rights reserved.
//

#include <future>
#include "RayTracer.hpp"
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
#include "image_texture.hpp"
#include "diffuse_light.hpp"
#include "xy_rect.hpp"
#include "box.hpp"
#include "translate.hpp"
#include "pdf.hpp"

#include "CoreFoundation/CoreFoundation.h"

//float hit_sphere_normals(const Vector3& center, double radius, const ray& r) {
//    Vector3 oc = r.origin() - center;
//    float a = Vector3::dotProduct(r.direction(), r.direction());
//    float b = 2.0 * Vector3::dotProduct(oc, r.direction());
//    float c = Vector3::dotProduct(oc, oc) - (radius * radius);
//    float discriminant = b*b - 4*a*c;
//    if (discriminant < 0) {
//        return -1;
//    } else {
//        return (-b - sqrt(discriminant)) / (2.0*a);
//    }
//}
//
//bool hit_sphere(const Vector3& center, double radius, const ray& r) {
//    Vector3 oc = r.origin() - center;
//    float a = Vector3::dotProduct(r.direction(), r.direction());
//    float b = 2.0 * Vector3::dotProduct(oc, r.direction());
//    float c = Vector3::dotProduct(oc, oc) - (radius * radius);
//    float discriminant = b*b - 4*a*c;
//    return (discriminant > 0);
//}
//
//
//Color color_normals(const ray &ray) {
//    float t = hit_sphere_normals(Vector3(0.0, 0.0, -1.0), 0.5, ray);
//    if (t > 0.0) {
//        Vector3 N = ray.parameterAtPoint(t) - Vector3(0,0,-1);
//        N = N.normalise();
//        Vector3 colorVector = 0.5 * (N + Vector3(1,1,1));
//        return Color(colorVector.x, colorVector.y, colorVector.z);
//    }
//    Vector3 unit_direction = ray.direction().normalise();
//    t = 0.5 * (unit_direction.y + 1.0);
//    Vector3 colorVector = (1.0 - t) * Vector3(1.0, 1.0, 1.0) + (t * Vector3(0.5, 0.7, 1.0));
//    return Color(colorVector.x, colorVector.y, colorVector.z);
//}
//
//Color color(const ray &ray) {
//    if (hit_sphere(Vector3(0.0, 0.0, -1.0), 0.5, ray)) {
//        return Color(1,0,0);
//    }
//    Vector3 unit_direction = ray.direction().normalise();
//    double t = 0.5 * (unit_direction.y + 1.0);
//    Vector3 colorVector = (1.0 - t) * Vector3(1.0, 1.0, 1.0) + (t * Vector3(0.5, 0.7, 1.0));
//    return Color(colorVector.x, colorVector.y, colorVector.z);
//}

//Vector3 random_in_unit_sphere() {
//    Vector3 p;
//    do {
//        p = 2.0 * Vector3(drand48(), drand48(), drand48()) - Vector3(1,1,1);
//    } while (p.squareMagnitude() >= 1.0);
//    return p;
//}

//Color color(const ray &r, hitable *world) {
//    hit_record rec;
//    if (world->hit(r, 0.001, MAXFLOAT, rec)) {
//        Vector3 target = rec.p + rec.normal + random_in_unit_sphere();
//        Color colorVector = 0.5 * color(ray(rec.p, target-rec.p), world);
//        return colorVector;
//    } else {
//        Vector3 unit_direction = r.direction().normalise();
//        double t = 0.5 * (unit_direction.y + 1.0);
//        Vector3 colorVector = (1.0 - t) * Vector3(1.0, 1.0, 1.0) + (t * Vector3(0.5, 0.7, 1.0));
//        return Color(colorVector.x, colorVector.y, colorVector.z);
//    }
//}

//Color color(const ray &r, hitable *world, int depth) {
//    hit_record rec;
//    if (world->hit(r, 0.001, MAXFLOAT, rec)) {
//        ray scattered;
//        Vector3 attenuation;
//        Color emitted = rec.mat_ptr->emitted(r, rec, rec.u, rec.v, rec.p);
//        float pdf_val;
//        Color albedo;
//        if (depth < 50 && rec.mat_ptr->scatter(r, rec, albedo, scattered, pdf_val)) {
//            hitable *light_shape = new xz_rect(213, 343, 227, 332, 554, 0);
//            hitable_pdf p0(light_shape, rec.p);
//            cosine_pdf p1(rec.normal);
//            mixture_pdf p(&p0, &p1);
//            scattered = ray(rec.p, p.generate(), r.time());
//            pdf_val = p.value(scattered.direction());
//            return emitted + albedo * rec.mat_ptr->scattering_pdf(r, rec, scattered) * color(scattered, world, depth+1) / pdf_val;
//        } else {
//            return emitted;
//        }
//    } else {
//        return Color(0,0,0);
//    }
//}

Color color(const ray &r, hitable *world, hitable *light_shape, int depth) {
    hit_record hrec;
    if (world->hit(r, 0.001, MAXFLOAT, hrec)) {
        scatter_record srec;
        Color emitted = hrec.mat_ptr->emitted(r, hrec, hrec.u, hrec.v, hrec.p);
        if (depth < 50 && hrec.mat_ptr->scatter(r, hrec, srec)) {
            if (srec.is_specular) {
                return srec.attenuation * color(srec.specular_ray, world, light_shape, depth+1);
            } else {
                hitable_pdf plight(light_shape, hrec.p);
                mixture_pdf p(&plight, srec.pdf_ptr);
                ray scattered = ray(hrec.p, p.generate(), r.time());
                float pdf_val = p.value(scattered.direction());
                delete srec.pdf_ptr;
                return emitted + srec.attenuation * hrec.mat_ptr->scattering_pdf(r, hrec, scattered) * color(scattered, world, light_shape, depth+1) / pdf_val;
            }
        } else {
            return emitted;
        }
    } else {
        return Color(0,0,0);
    }
}

hitable *simple_light() {
    texture *pertext = new noise_texture(4);
    material *light = new diffuse_light(new constant_texture(Color(4,4,4)));
    hitable **list = new hitable*[4];
    list[0] = new sphere(Vector3(0,-1000,0),1000, new lambertian(pertext));
    list[1] = new sphere(Vector3(0,2,0),2, new lambertian(pertext));
    list[2] = new sphere(Vector3(0,7,0),2, light);
    list[3] = new xy_rect(3,5,1,3,-2, light);
    return new hitable_list(list,4);
}

hitable *cornell_box() {
    material *red   = new lambertian( new constant_texture(Color(0.65, 0.05, 0.05)) );
    material *white = new lambertian( new constant_texture(Color(0.73, 0.73, 0.73)) );
    material *green = new lambertian( new constant_texture(Color(0.12, 0.45, 0.15)) );
    material *light = new diffuse_light( new constant_texture(Color(15, 15, 15)) );
    material *aluminium = new metal(Color(0.8,0.85,0.88), 0.0);
    hitable **list = new hitable*[8];
    int i = 0;
    list[i++] = new flip_normals(new yz_rect(0,555,0,555,555, green));
    list[i++] = new yz_rect(0,555,0,555,0, red);
    list[i++] = new flip_normals(new xz_rect(213,343,227,332,554, light));
    list[i++] = new flip_normals(new xz_rect(0,555,0,555,555, white));
    list[i++] = new xz_rect(0,555,0,555,0, white);
    list[i++] = new flip_normals(new xy_rect(0,555,0,555,555, white));
    
    list[i++] = new translate(new rotate_y(new box(Vector3(0,0,0), Vector3(165,165,165), white), -18), Vector3(130, 0, 65));
    list[i++] = new translate(new rotate_y(new box(Vector3(0,0,0), Vector3(165,330,165), aluminium), 15), Vector3(265,0,295));
    
    return new hitable_list(list,i);
}

hitable *cornell_box_with_sphere() {
    material *red   = new lambertian( new constant_texture(Color(0.65, 0.05, 0.05)) );
    material *white = new lambertian( new constant_texture(Color(0.73, 0.73, 0.73)) );
    material *green = new lambertian( new constant_texture(Color(0.12, 0.45, 0.15)) );
    material *light = new diffuse_light( new constant_texture(Color(15, 15, 15)) );
    material *aluminium = new metal(Color(0.8,0.85,0.88), 0.0);
    material *glass = new dielectric(1.5);
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
    
    return new hitable_list(list,i);
}


hitable *random_scene() {
    
    texture *checkerTexture = new checker_texture(new constant_texture(Color(0.2,0.3,0.1)), new constant_texture(Color(0.9,0.9,0.9)));
    texture *noiseTexture = new noise_texture(10);
    
    int n = 500;
    hitable **list = new hitable*[n + 1];
    list[0] = new sphere(Vector3(0,-1000,-0), 1000,new lambertian(noiseTexture));
    int i = 1;
    
//    for (int a = -4; a < 4; a++) {
//        for (int b = -4; b < 4; b++) {
//            float choose_mat = drand48();
//            Vector3 center(a+0.9*drand48(), 0.2, b+0.9*drand48());
//            if ((center-Vector3(4,0.2,0)).length() > 0.9) {
//                if (choose_mat < 0.8) {
//                    // diffuse
//                    texture *tex = new constant_texture(Color(drand48()*drand48(), drand48()*drand48(), drand48()*drand48()));
//                    list[i++] = new moving_sphere(center, center + Vector3(0, 0.3, 0), 0, 1, 0.2, new lambertian(tex));
//                } else if (choose_mat < 0.95) {
//                    // metal
//                    list[i++] = new sphere(center, 0.2, new metal(Vector3(0.5*(1+drand48()), 0.5*(1+drand48()), 0.5*(1+drand48())), 0.01));
//                } else {
//                    // glass
//                    list[i++] = new sphere(center, 0.2, new dielectric(1.5));
//                }
//            }
//        }
//    }
    
    
    //texture *colorTexture = new constant_texture(Color(0.4,0.2,0.1));
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef resourcesURL = CFBundleCopyResourceURL(mainBundle, CFSTR("1_earth.jpg"), NULL, NULL);
    char path[PATH_MAX];
    if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, PATH_MAX)){
        // error!
    }
    CFRelease(resourcesURL);
    int nx, ny, nn;
    unsigned char *tex_data = stbi_load(path, &nx, &ny, &nn, 0);
    texture *earthTexture = new image_texture(tex_data, nx, ny);
    
    list[i++] = new sphere(Vector3(0,1,0), 1.0, new dielectric(1.5));
    list[i++] = new sphere(Vector3(4,1,0), 1.0, new lambertian(earthTexture));
    //list[i++] = new sphere(Vector3(-4,1,0), 1.0, new metal(Vector3(0.7, 0.6, 0.5), 0.01));
    
    return new hitable_list(list, i);
}

inline Color de_nan(const Color& c) {
    Color temp = c;
    if (isnan(temp.r)) temp.r = 0;
    if (isnan(temp.g)) temp.g = 0;
    if (isnan(temp.b)) temp.b = 0;
    return temp;
}

ImageBuffer* RayTracer::render() {
    
    ImageBuffer *imageBuffer = new ImageBuffer(width, height, Color(0,0,0));
    
    texture *checkerTexture = new checker_texture(new constant_texture(Color(0.2,0.3,0.1)), new constant_texture(Color(0.9,0.9,0.9)));
    
    hitable *list[4];
    list[0] = new sphere(Vector3(0,0,-1), 0.5, new lambertian(new constant_texture(Color(0.1,0.2,0.5))));
    list[1] = new sphere(Vector3(0,-100.5,-1), 100, new lambertian(checkerTexture));
    list[2] = new sphere(Vector3(1,0,-1), 0.5, new metal(Color(0.8, 0.6, 0.2), 0.01));
    list[3] = new sphere(Vector3(-1,0,-1), 0.5, new dielectric(1.5));
    //hitable *world = new hitable_list(list, 4);
    
    //hitable *world = random_scene();
    //hitable *world = simple_light();
    //hitable *world = cornell_box();
    hitable *world = cornell_box_with_sphere();
    
    hitable *light_shape = new xz_rect(213, 343, 227, 332, 554, nullptr);
    hitable *glass_sphere = new sphere(Vector3(190, 90, 190), 90, nullptr);
    //hitable *block_shape = new translate(new rotate_y(new box(Vector3(0,0,0), Vector3(165,330,165), nullptr), 15), Vector3(265,0,295));
    hitable *a[2];
    a[0] = light_shape;
    a[1] = glass_sphere;
    //a[1] = block_shape;
    hitable_list *hlist = new hitable_list(a,2);
    
    //camera *cam = new cameraA(90, float(width)/float(height));
    
    Vector3 lookfrom(278,278,-800);
    Vector3 lookat(278,278,0);
    float dist_to_focus = 10.0;
    float aperture = 0.0;
    float vfov = 40.0;
    camera *cam = new cameraC(lookfrom, lookat, Vector3(0,1,0), vfov, float(width)/float(height), aperture, dist_to_focus, 0.0, 1.0);
    
    std::size_t max = width * height;
    std::size_t cores = std::thread::hardware_concurrency();
    volatile std::atomic<std::size_t> count(0);
    std::vector<std::future<void>> future_vector;
    while (cores--) {
        std::future<void> future = std::async(std::launch::async, [=, &imageBuffer, &count]
                                              {
                                                  while (true)
                                                  {
                                                      std::size_t index = count++;
                                                      if (index >= max)
                                                          break;
                                                      std::size_t i = index % width;
                                                      std::size_t j = index / width;
                                                      Color col(0,0,0);
                                                      for(int s = 0; s < ns; s++) {
                                                          double u = float(i + drand48()) / float(width);
                                                          double v = float(j + drand48()) / float(height);
                                                          ray ray = cam->get_ray(u, v);
                                                          Vector3 p = ray.parameterAtPoint(2.0);
                                                          Color col2 = color(ray, world, hlist, 0);
                                                          col += de_nan(col2);
                                                      }
                                                      
                                                      
                                                      col /= float(ns);
                                                      col = Color(sqrt(col.r), sqrt(col.g), sqrt(col.b));
                                                      
                                                      imageBuffer->pixels[i + j*width] = col;
                                                  }
                                              });
        //future.wait();
        future_vector.emplace_back(std::move(future));
    }
    
//    for (int i = 0; i < width; i++) {
//        for (int j = 0; j < height; j++) {
//            Color col(0,0,0);
//            for(int s = 0; s < ns; s++) {
//                double u = float(i + drand48()) / float(width);
//                double v = float(j + drand48()) / float(height);
//                ray ray = cam->get_ray(u, v);
//                Vector3 p = ray.parameterAtPoint(2.0);
//                col += de_nan(color(ray, world, &hlist, 0));
//            }
//
//
//            col /= float(ns);
//            col = Color(sqrt(col.r), sqrt(col.g), sqrt(col.b));
//
//            imageBuffer->pixels[i + j*width] = col;
//        }
//
//    }
    
    
    
    return imageBuffer;
}
