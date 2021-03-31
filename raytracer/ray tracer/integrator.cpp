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
#include "aabb.hpp"

inline Color de_nan(const Color& c) {
    Color temp = c;
    if (isnan(temp.r())) temp.r() = 0;
    if (isnan(temp.g())) temp.g() = 0;
    if (isnan(temp.b())) temp.b() = 0;
    return temp;
}


void TestIntegrator::render(const Scene &scene) {
    
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
                                                      for(int s = 0; s < ns; ++s) {
                                                          double u = float(i + drand48()) / float(width);
                                                          double v = float(j + drand48()) / float(height);
                                                          Ray ray = scene.camera->get_ray(u, v);
                                                          Color col2 = color(ray, scene.world.get(), scene.light_shape, scene.sky_box.get(), 0);
                                                          col += de_nan(col2);
                                                      }
                                                      
                                                      
                                                      col /= float(ns);
                                                      col = Color(sqrt(col.r()), sqrt(col.g()), sqrt(col.b()));
                                                      imageBuffer->setColor(i, j, col);
                                                  }
                                              });
        future_vector.emplace_back(std::move(future));
    }
}


Color TestIntegrator::color(const Ray &r, hitable *world, hitable *light_shape, SkyBox *sky_box, int depth) {
    SurfaceInteraction hrec;
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
                Ray scattered;
                float pdf_val;
                do {
                   scattered = Ray(hrec.p, p.generate(), r.time());
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

Vector3f LessThan(Vector3f f, float value)
{
    return Vector3f((f.x() < value) ? 1.0f : 0.0f,
                    (f.y() < value) ? 1.0f : 0.0f,
                    (f.z() < value) ? 1.0f : 0.0f);
}

Vector3f clamp(Vector3f f, float min, float max)
{
    return Vector3f(ffclamp(f.x(), min, max),
                    ffclamp(f.y(), min, max),
                    ffclamp(f.z(), min, max));
}

Vector3f mix(Vector3f a, Vector3f b, Vector3f t) {
    float x = (1.0f - t.x()) * a.x() + t.x() * b.x();
    float y = (1.0f - t.y()) * a.y() + t.y() * b.y();
    float z = (1.0f - t.z()) * a.z() + t.z() * b.z();
    return Vector3f(x,y,z);
}

Vector3f pow(Vector3f a, Vector3f exp) {
    float x = std::powf(a.x(), exp.x());
    float y = std::powf(a.y(), exp.y());
    float z = std::powf(a.z(), exp.z());
    return Vector3f(x,y,z);
}

Vector3f LinearToSRGB(Vector3f rgb)
{
    rgb = clamp(rgb, 0.0f, 1.0f);
     
    return mix(
        pow(rgb, Vector3f(1.0f / 2.4f)) * 1.055f - 0.055f,
        rgb * 12.92f,
        LessThan(rgb, 0.0031308f)
    );
}
 
Vector3f SRGBToLinear(Vector3f rgb)
{
    return rgb;
     
    rgb = clamp(rgb, 0.0f, 1.0f);
     
    return mix(
        pow(((rgb + 0.055f) / 1.055f), Vector3f(2.4f)),
        rgb / 12.92f,
        LessThan(rgb, 0.04045f)
    );
}

Vector3f ACESFilm(Vector3f x)
{
    float a = 2.51f;
    float b = 0.03f;
    float c = 2.43f;
    float d = 0.59f;
    float e = 0.14f;
    
    return clamp((x*(a*x + b)) / (x*(c*x + d) + e), 0.0f, 1.0f);
}

void TestIntegrator2::render(const Scene &scene) {
    
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
                                                      if (index >= max) {
                                                          count = 0;
                                                          index = 0;
                                                      }
                                                      std::size_t i = index % width;
                                                      std::size_t j = index / width;
                                                      if (i == 0 && j%10==0) {
                                                          std::cout << j << std::endl;
                                                      }
                                                      Color col = totalsImageBuffer->getColor(i, j);
                                                      int ns_here = 10;
                                                      for(int s = 0; s < ns_here; ++s) {
                                                          double u = float(i + drand48()) / float(width);
                                                          double v = float(j + drand48()) / float(height);
                                                          Ray ray = scene.camera->get_ray(u, v);
                                                          Color col2 = color(ray, scene.world.get(), scene.light_shape, scene.sky_box.get(), 0);
                                                          col += de_nan(col2);
                                                      }
                                                      totalsImageBuffer->setColor(i, j, col);
                                                      ns_pix[i + j*width] += ns_here;
                                                      size_t ns2 = ns_pix[i + j*width];
                                                      col /= float(ns2);
                                                      col = Color(_mm_sqrt_ps(col._a));
                                                      
                                                      // apply exposure (how long the shutter is open)
                                                      //float c_exposure = 0.5;
                                                      //col *= c_exposure;
                                                      
                                                      // convert unbounded HDR color range to SDR color range
                                                      //Vector3f aces = ACESFilm(col);
                                                      // convert from linear to sRGB for display
                                                      //Vector3f lin = LinearToSRGB(aces);
                                                      //col = Color(lin._a);
                                                      
                                                      imageBuffer->setColor(i, j, col);
                                                  }
                                              });
        future_vector.emplace_back(std::move(future));
    }
}


Color TestIntegrator2::color(const Ray &r, hitable *world, hitable *light_shape, SkyBox *sky_box, int depth) {
    SurfaceInteraction hrec;
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
                Ray scattered;
                float pdf_val;
                do {
                    scattered = Ray(hrec.p, p.generate(), r.time());
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







void BasicIntegrator::render(const Scene &scene) {
    
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
                                                      for(int s = 0; s < ns; ++s) {
                                                          double u = float(i + drand48()) / float(width);
                                                          double v = float(j + drand48()) / float(height);
                                                          Ray ray = scene.camera->get_ray(u, v);
                                                          //Vector3f p = ray.pointAtParameter(2.0);
                                                          Color col2 = color(ray, scene.world.get(), scene.light_shape, scene.sky_box.get(), 0);
                                                          col += de_nan(col2);
                                                      }
                                                      
                                                      
                                                      col /= float(ns);
                                                      col = Color(_mm_sqrt_ps(col._a));
                                                      
                                                      imageBuffer->setColor(i, j, col);
                                                  }
                                              });
        future_vector.emplace_back(std::move(future));
    }
}

Color BasicIntegrator::color(const Ray &r, hitable *world, hitable *light_shape, SkyBox *sky_box, int depth) {
    SurfaceInteraction hrec;
    if (world->hit(r, 0.001, MAXFLOAT, hrec)) {
        scatter_record srec;
        Color emitted = hrec.mat_ptr->emitted(r, hrec, hrec.u, hrec.v, hrec.p);
        if (depth < 50 && hrec.mat_ptr->scatter(r, hrec, srec)) {
            Ray scattered = srec.specular_ray;
            scattered = Ray(hrec.p, srec.pdf_ptr->generate(), r.time());
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







void JitterIntegrator::render(const Scene &scene) {
    
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
                                                      for(int s = 0; s < ns; ++s) {
                                                          for (int t = 0; t < ns; ++t) {
                                                              
                                                              double originX = i;
                                                              double originY = j;
                                                              double posX = static_cast<double>(s) / ns;
                                                              double posY = static_cast<double>(t) / ns;
                                                              
                                                              double u = (originX + posX + drand48() / ns) / width;
                                                              double v = (originY + posY + drand48() / ns) / height;
                                                              Ray ray = scene.camera->get_ray(u, v);
                                                              Color col2 = color(ray, scene.world.get(), scene.light_shape, scene.sky_box.get(), 0);
                                                              col += de_nan(col2);
                                                              
                                                              
                                                          }
                                                      }
                                                      
                                                      
                                                      col /= float(ns*ns);
                                    
                                                      col = Color(_mm_sqrt_ps(col._a));
                                                      
                                                      imageBuffer->setColor(i, j, col);
                                                  }
                                              });
        future_vector.emplace_back(std::move(future));
    }
}


Color JitterIntegrator::color(const Ray &r, hitable *world, hitable *light_shape, SkyBox *sky_box, int depth) {
    SurfaceInteraction hrec;
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
                Ray scattered;
                float pdf_val;
                do {
                    scattered = Ray(hrec.p, p.generate(), r.time());
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


#define DEPTH_NO_HIT -1.0

void DepthIntegrator::render(const Scene &scene) {
    int width  = imageBuffer->width;
    int height = imageBuffer->height;
    
    std::size_t max = width * height;
    std::size_t count = 0;
    
    float minD = MAXFLOAT;
    float maxD = 0.0;
    
    for (int a = 0; a < width * height; a++)
      {
          std::size_t index = count++;
//          if (index >= max)
//              break;
          std::size_t i = index % width;
          std::size_t j = index / width;
          if (i == 0 && j%10==0) {
              std::cout << j << std::endl;
          }
          Color col(1,1,1);
          double u = float(i + drand48()) / float(width);
          double v = float(j + drand48()) / float(height);
          Ray ray = scene.camera->get_ray(u, v);
          float d = dist(ray, scene.world.get(), min, max);
          if (d != DEPTH_NO_HIT) {
              minD = std::min(minD, d);
              maxD = std::max(maxD, d);
              
              printf("min: %f, max: %f\n", minD, maxD);
              
              float f = (d - min) / depth;
              col = Color(f,f,f);
          }
          
          
          imageBuffer->setColor(i, j, col);
      }
}

float DepthIntegrator::dist(const Ray &r, hitable *world, float min, float max) {
    SurfaceInteraction hrec;
    if (world->hit(r, 0.001, MAXFLOAT, hrec)) {
        printf("t = %f", hrec.t);
        return hrec.t;
    } else {
        return DEPTH_NO_HIT;
    }
}
