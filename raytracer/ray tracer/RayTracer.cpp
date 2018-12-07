//
//  RayTracer.cpp
//  raytracer
//
//  Created by Adrian Russell on 29/06/2018.
//  Copyright © 2018 Adrian Russell. All rights reserved.
//

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

Color color(const ray &r, hitable *world, int depth) {
    hit_record rec;
    if (world->hit(r, 0.001, MAXFLOAT, rec)) {
        ray scattered;
        Vector3 attenuation;
        if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            return color(scattered, world, depth+1) * attenuation;
        } else {
            return Color(0,0,0);
        }
    } else {
        Vector3 unit_direction = r.direction().normalise();
        double t = 0.5 * (unit_direction.y + 1.0);
        Vector3 colorVector = (1.0 - t) * Vector3(1.0, 1.0, 1.0) + (t * Vector3(0.5, 0.7, 1.0));
        return Color(colorVector.x, colorVector.y, colorVector.z);
    }
}


hitable *random_scene() {
    int n = 500;
    hitable **list = new hitable*[n + 1];
    list[0] = new sphere(Vector3(0,-1000,-0), 1000, new lambertian(Vector3(0.5, 0.5, 0.5)));
    int i = 1;
    
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            float choose_mat = drand48();
            Vector3 center(a+0.9*drand48(), 0.2, b+0.9*drand48());
            if ((center-Vector3(4,0.2,0)).length() > 0.9) {
                if (choose_mat < 0.8) {
                    // diffuse
                    list[i++] = new sphere(center, 0.2, new lambertian(Vector3(drand48()*drand48(), drand48()*drand48(), drand48()*drand48())));
                } else if (choose_mat < 0.95) {
                    // metal
                    list[i++] = new sphere(center, 0.2, new metal(Vector3(0.5*(1+drand48()), 0.5*(1+drand48()), 0.5*(1+drand48())), 0.01));
                } else {
                    // glass
                    list[i++] = new sphere(center, 0.2, new dielectric(1.5));
                }
            }
        }
    }
    
    list[i++] = new sphere(Vector3(0,1,0), 1.0, new dielectric(1.5));
    list[i++] = new sphere(Vector3(-4,1,0), 1.0, new lambertian(Vector3(0.4, 0.2, 0.1)));
    list[i++] = new sphere(Vector3(4,1,0), 1.0, new metal(Vector3(0.7, 0.6, 0.5), 0.01));
    
    return new hitable_list(list, i);
}



ImageBuffer* RayTracer::render() {
    
    ImageBuffer *imageBuffer = new ImageBuffer(width, height, Color(0,0,0));
    
    hitable *list[4];
    list[0] = new sphere(Vector3(0,0,-1), 0.5, new lambertian(Vector3(0.1, 0.2, 0.5)));
    list[1] = new sphere(Vector3(0,-100.5,-1), 100, new lambertian(Vector3(0.8, 0.8, 0.0)));
    list[2] = new sphere(Vector3(1,0,-1), 0.5, new metal(Vector3(0.8, 0.6, 0.2), 0.01));
    list[3] = new sphere(Vector3(-1,0,-1), 0.5, new dielectric(1.5));
    //hitable *world = new hitable_list(list, 4);
    
    hitable *world = random_scene();
    
    //camera *cam = new cameraA(90, float(width)/float(height));
    
    Vector3 lookfrom(15,3,2);
    Vector3 lookat(0,0,-1);
    float dist_to_focus = (lookfrom-lookat).length();
    float aperture = 0.5;
    camera *cam = new cameraC(lookfrom, lookat, Vector3(0,1,0), 20, float(width)/float(height), aperture, dist_to_focus);
    
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            Color col(0,0,0);
            for(int s = 0; s < ns; s++) {
                double u = float(i + drand48()) / float(width);
                double v = float(j + drand48()) / float(height);
                ray ray = cam->get_ray(u, v);
                Vector3 p = ray.parameterAtPoint(2.0);
                col += color(ray, world, 0);
            }
            
            
            col /= float(ns);
            col = Color(sqrt(col.r), sqrt(col.g), sqrt(col.b));
            
            imageBuffer->pixels[i + j*width] = col;
        }
        
    }
    
    
    
    return imageBuffer;
}
