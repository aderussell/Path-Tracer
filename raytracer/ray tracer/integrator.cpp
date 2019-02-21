//
//  integrator.cpp
//  raytracer
//
//  Created by Adrian Russell on 27/01/2019.
//  Copyright © 2019 Adrian Russell. All rights reserved.
//

#include "integrator.hpp"
#include <future>
#include <iostream>

inline Color de_nan(const Color& c) {
    Color temp = c;
    if (isnan(temp.r)) temp.r = 0;
    if (isnan(temp.g)) temp.g = 0;
    if (isnan(temp.b)) temp.b = 0;
    return temp;
}


void TestIntegrator::render(const scene &scene) {
    
    int width  = imageBuffer->width;
    int height = imageBuffer->height;
    
    std::size_t max = width * height;
    std::size_t cores = std::thread::hardware_concurrency();
    volatile std::atomic<std::size_t> count(0);
    std::vector<std::future<void>> future_vector;
    while (cores--) {
        std::future<void> future = std::async(std::launch::async, [=, &scene, &width, &height, &count]
                                              {
                                                  while (true)
                                                  {
                                                      std::size_t index = count++;
                                                      if (index >= max)
                                                          break;
                                                      std::size_t i = index % width;
                                                      std::size_t j = index / width;
                                                      if (i == 0 && j%10==0) {
                                                          std::cout << j << std::endl;
                                                      }
                                                      Color col(0,0,0);
                                                      for(int s = 0; s < ns; s++) {
                                                          double u = float(i + drand48()) / float(width);
                                                          double v = float(j + drand48()) / float(height);
                                                          ray ray = scene.camera->get_ray(u, v);
                                                          Vector3f p = ray.parameterAtPoint(2.0);
                                                          Color col2 = color(ray, scene.world, scene.light_shape, scene.sky_box, 0);
                                                          col += de_nan(col2);
                                                      }
                                                      
                                                      
                                                      col /= float(ns);
                                                      col = Color(sqrt(col.r), sqrt(col.g), sqrt(col.b));
                                                      
                                                      imageBuffer->pixels[i + j*width] = col;
                                                  }
                                              });
        future_vector.emplace_back(std::move(future));
    }
}


Color TestIntegrator::color(const ray &r, hitable *world, hitable *light_shape, skybox *sky_box, int depth) {
    hit_record hrec;
    if (world->hit(r, 0.001, MAXFLOAT, hrec)) {
        scatter_record srec;
        Color emitted = hrec.mat_ptr->emitted(r, hrec, hrec.u, hrec.v, hrec.p);
        if (depth < 50 && hrec.mat_ptr->scatter(r, hrec, srec)) {
            if (srec.is_specular) {
                delete srec.pdf_ptr;
                return srec.attenuation * color(srec.specular_ray, world, light_shape, sky_box, depth+1);
            } else {
                hitable_pdf plight(light_shape, hrec.p);
                pdf *plightP = (light_shape != nullptr) ? &plight : srec.pdf_ptr;
                mixture_pdf p(plightP, srec.pdf_ptr);
                ray scattered;
                float pdf_val;
                do {
                   scattered = ray(hrec.p, p.generate(), r.time());
                   pdf_val = p.value(scattered.direction());
                } while (pdf_val < 1E-13);
                delete srec.pdf_ptr;
                return emitted + srec.attenuation * hrec.mat_ptr->scattering_pdf(r, hrec, scattered) * color(scattered, world, light_shape, sky_box, depth+1) / pdf_val;
            }
        } else {
            delete srec.pdf_ptr;
            return emitted;
        }
    } else {
        return sky_box->get_color(r);
    }
}




void BasicIntegrator::render(const scene &scene) {
    
    int width  = imageBuffer->width;
    int height = imageBuffer->height;
    
    std::size_t max = width * height;
    std::size_t cores = std::thread::hardware_concurrency();
    volatile std::atomic<std::size_t> count(0);
    std::vector<std::future<void>> future_vector;
    while (cores--) {
        std::future<void> future = std::async(std::launch::async, [=, &scene, &width, &height, &count]
                                              {
                                                  while (true)
                                                  {
                                                      std::size_t index = count++;
                                                      if (index >= max)
                                                          break;
                                                      std::size_t i = index % width;
                                                      std::size_t j = index / width;
                                                      if (i == 0 && j%10==0) {
                                                          std::cout << j << std::endl;
                                                      }
                                                      Color col(0,0,0);
                                                      for(int s = 0; s < ns; s++) {
                                                          double u = float(i + drand48()) / float(width);
                                                          double v = float(j + drand48()) / float(height);
                                                          ray ray = scene.camera->get_ray(u, v);
                                                          Vector3f p = ray.parameterAtPoint(2.0);
                                                          Color col2 = color(ray, scene.world, scene.light_shape, scene.sky_box, 0);
                                                          col += de_nan(col2);
                                                      }
                                                      
                                                      
                                                      col /= float(ns);
                                                      col = Color(sqrt(col.r), sqrt(col.g), sqrt(col.b));
                                                      
                                                      imageBuffer->pixels[i + j*width] = col;
                                                  }
                                              });
        future_vector.emplace_back(std::move(future));
    }
}

Color BasicIntegrator::color(const ray &r, hitable *world, hitable *light_shape, skybox *sky_box, int depth) {
    hit_record hrec;
    if (world->hit(r, 0.001, MAXFLOAT, hrec)) {
        scatter_record srec;
        Color emitted = hrec.mat_ptr->emitted(r, hrec, hrec.u, hrec.v, hrec.p);
        if (depth < 50 && hrec.mat_ptr->scatter(r, hrec, srec)) {
            ray scattered = srec.specular_ray;
            scattered = ray(hrec.p, srec.pdf_ptr->generate(), r.time());
            delete srec.pdf_ptr;
            return emitted + srec.attenuation * color(scattered, world, light_shape, sky_box, depth+1);
        } else {
            delete srec.pdf_ptr;
            return emitted;
        }
    } else {
        return sky_box->get_color(r);
    }
}







void JitterIntegrator::render(const scene &scene) {
    
    int width  = imageBuffer->width;
    int height = imageBuffer->height;
    
    std::size_t max = width * height;
    std::size_t cores = std::thread::hardware_concurrency();
    volatile std::atomic<std::size_t> count(0);
    std::vector<std::future<void>> future_vector;
    while (cores--) {
        std::future<void> future = std::async(std::launch::async, [=, &scene, &width, &height, &count]
                                              {
                                                  while (true)
                                                  {
                                                      std::size_t index = count++;
                                                      if (index >= max)
                                                          break;
                                                      std::size_t i = index % width;
                                                      std::size_t j = index / width;
                                                      if (i == 0 && j%10==0) {
                                                          std::cout << j << std::endl;
                                                      }
                                                      Color col(0,0,0);
                                                      for(int s = 0; s < ns; s++) {
                                                          for (int t = 0; t < ns; t++) {
                                                              
                                                              double originX = i;
                                                              double originY = j;
                                                              double posX = static_cast<double>(s) / ns;
                                                              double posY = static_cast<double>(t) / ns;
                                                              
                                                              double u = (originX + posX + drand48() / ns) / width;
                                                              double v = (originY + posY + drand48() / ns) / height;
                                                              ray ray = scene.camera->get_ray(u, v);
                                                              Vector3f p = ray.parameterAtPoint(2.0);
                                                              Color col2 = color(ray, scene.world, scene.light_shape, scene.sky_box, 0);
                                                              col += de_nan(col2);
                                                              
                                                              
                                                          }
                                                      }
                                                      
                                                      
                                                      col /= float(ns*ns);
                                                      col = Color(sqrt(col.r), sqrt(col.g), sqrt(col.b));
                                                      
                                                      imageBuffer->pixels[i + j*width] = col;
                                                  }
                                              });
        future_vector.emplace_back(std::move(future));
    }
}


Color JitterIntegrator::color(const ray &r, hitable *world, hitable *light_shape, skybox *sky_box, int depth) {
    hit_record hrec;
    if (world->hit(r, 0.001, MAXFLOAT, hrec)) {
        scatter_record srec;
        Color emitted = hrec.mat_ptr->emitted(r, hrec, hrec.u, hrec.v, hrec.p);
        if (depth < 50 && hrec.mat_ptr->scatter(r, hrec, srec)) {
            if (srec.is_specular) {
                delete srec.pdf_ptr;
                return srec.attenuation * color(srec.specular_ray, world, light_shape, sky_box, depth+1);
            } else {
                hitable_pdf plight(light_shape, hrec.p);
                pdf *plightP = (light_shape != nullptr) ? &plight : srec.pdf_ptr;
                mixture_pdf p(plightP, srec.pdf_ptr);
                ray scattered;
                float pdf_val;
                do {
                    scattered = ray(hrec.p, p.generate(), r.time());
                    pdf_val = p.value(scattered.direction());
                } while (pdf_val < 1E-13);
                delete srec.pdf_ptr;
                return emitted + srec.attenuation * hrec.mat_ptr->scattering_pdf(r, hrec, scattered) * color(scattered, world, light_shape, sky_box, depth+1) / pdf_val;
            }
        } else {
            delete srec.pdf_ptr;
            return emitted;
        }
    } else {
        return sky_box->get_color(r);
    }
}
