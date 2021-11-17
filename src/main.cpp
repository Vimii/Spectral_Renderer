#include <iostream>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../stb-master/stb_image_write.h"
#include <memory>
#include <new>
#include "time.h"
#include "util.h"
#include "filename.h"
#include "spectrum.h"
#include "d65.h"
#include "hittable.h"
#include "material.h"
#include "camera.h"
#include "hittable_list.h"
#include "aarect.h"
#include "sphere.h"
#include "box.h"
#include <string.h>
#include <omp.h>


hittable_list cornell_box(camera& cam, double aspect) {
    hittable_list world;

    auto red   = make_shared<lambertian>(make_shared<solid_color>(.65, .05, .05));
    auto white = make_shared<lambertian>(make_shared<solid_color>(.73, .73, .73));
    auto green = make_shared<lambertian>(make_shared<solid_color>(.12, .45, .15));
    auto light = make_shared<diffuse_light>(make_shared<solid_color>(15, 15, 15));
    shared_ptr<material> aluminum =
            make_shared<metal>(RED, 0.0);

    world.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
    world.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    world.add(make_shared<flip_face>(make_shared<xz_rect>(213, 343, 227, 332, 554, light)));
    world.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    world.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    world.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));

    shared_ptr<hittable> box1 = make_shared<box>(point3(0,0,0), point3(165,330,165), aluminum);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, vec3(265,0,295));
    world.add(box1);

    auto glass = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(190, 90, 190), 90, glass));

    point3 lookfrom(278, 278, -800);
    point3 lookat(278, 278, 0);
    vec3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.0;
    auto vfov = 40.0;
    auto t0 = 0.0;
    auto t1 = 1.0;

    cam = camera(lookfrom, lookat, vup, vfov, aspect, aperture, dist_to_focus, t0, t1);

    return world;
}

//color ray_color(const ray& r, const color& background, int depth) {
//    vec3 unit_direction = unit_vector(r.direction());
//    auto t = 0.5*(unit_direction.y() + 1.0);
//    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
//}

//spectrum ray_spectrum(const ray& r, int depth){
//    vec3 unit_direction = unit_vector(r.direction());
//    auto t = 0.5*(unit_direction.y() + 1.0);
//    spectrum ray;
//    ray = D65 * (t*BLUE + (1.0 - t)*RED)*10.f;
//    return ray;
//}

spectrum ray_spectrum(const ray& r, const spectrum& background, const hittable& world,shared_ptr<hittable> lights, int depth) {
    hit_record rec;

    // 反射回数が一定よりも多くなったら、その時点で追跡をやめる
    if (depth <= 0)
        return spectrum_type(_EMPTY);

    // レイがどのオブジェクトとも交わらないなら、背景色を返す
    if (!world.hit(r, 0.001, infinity, rec))
        return background;

    scatter_record srec;
    spectrum emitted = rec.mat_ptr->emitted(r, rec, rec.u, rec.v, rec.p);

    if (!rec.mat_ptr->scatter(r, rec, srec))
        return emitted;

    if(srec.is_specular){
        return srec.attenuation * ray_spectrum(srec.specular_ray, background, world, lights, depth-1);
    }

    shared_ptr<pdf> light_ptr = make_shared<hittable_pdf>(lights, rec.p);
    mixture_pdf p(light_ptr,srec.pdf_ptr);

    ray scattered = ray(rec.p,p.generate(),r.time());
    auto pdf_val = p.value(scattered.direction());

    return emitted
           + srec.attenuation * rec.mat_ptr->scattering_pdf(r, rec, scattered)
             * ray_spectrum(scattered, background, world, lights, depth-1) / pdf_val;
}


int main() {

    const auto aspect_ratio = 1.0;
    const int image_width = 500;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int sample_per_pixel = 500;
    const int max_depth = 50;
    const spectrum background = const_spectrum(0);

    std::unique_ptr<RGBA[][image_width]> rgba(new(std::nothrow) RGBA[image_height][image_width]);
    if (!rgba) return -1;

    camera cam;
    hittable_list world = cornell_box(cam,aspect_ratio);
    auto lights = make_shared<hittable_list>();
    lights->add(make_shared<xz_rect>(213, 343, 227, 332, 554, nullptr));
    lights->add(make_shared<sphere>(point3(190, 90, 190), 90, nullptr));

    clock_t start, stop;
    start = clock();

    for(int j = image_height -1 ; j >= 0; --j){
        std::cerr << "\rScanlines remaining: "  << j << ' ' << std::flush;
#pragma omp parallel for
        for(int i = 0; i < image_width; ++i){
            color pixel_color(0,0,0);
            spectrum pixel_spectrum;
        #pragma omp parallel for
            for(int s = 0; s < sample_per_pixel; ++s){
                auto u = (i + random_double())/(image_width-1);
                auto v = (j + random_double())/(image_height-1);
                ray r = cam.get_ray(u,v);
                pixel_spectrum += ray_spectrum(r,background, world, lights, max_depth);
            }
            pixel_color = xyz_to_srgb(spectrum_to_xyz(pixel_spectrum));
            write_color(rgba[image_height-1-j][i], pixel_color, sample_per_pixel);
        }
    }
    stop = clock();
    double timer_seconds = ((double)(stop - start)) / CLOCKS_PER_SEC;
    std::cerr << "\nDone.\n";
    std::cerr << "took " << timer_seconds << " seconds.\n";
    stbi_write_png(dayName().c_str(), static_cast<int>(image_width), static_cast<int>(image_height), static_cast<int>(sizeof(RGBA)), rgba.get(), 0);
}
