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
#include "loadObj.h"
#include "ObjModel.h"
#include <string.h>
#include <omp.h>
#include <iomanip>
#include "world.h"

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

float ray_spectrum(const ray& r, const spectrum& background, const hittable& world,shared_ptr<hittable> lights, int depth) {
    hit_record rec;

    // 反射回数が一定よりも多くなったら、その時点で追跡をやめる
    if (depth <= 0)
        return spectrum_type(_EMPTY)[getIndex(r.wavelength())];

    // レイがどのオブジェクトとも交わらないなら、背景色を返す
    if (!world.hit(r, 0.001, infinity, rec))
        return background[getIndex(r.wavelength())];

    scatter_record srec;
    spectrum emitted = rec.mat_ptr->emitted(r, rec, rec.u, rec.v, rec.p);

    if (!rec.mat_ptr->scatter(r, rec, srec))
        return emitted[getIndex(r.wavelength())];

    if(srec.is_specular){
        return srec.attenuation[getIndex(r.wavelength())] * ray_spectrum(srec.specular_ray, background, world, lights, depth-1);
    }

    shared_ptr<pdf> light_ptr = make_shared<hittable_pdf>(lights, rec.p);
    mixture_pdf p(light_ptr,srec.pdf_ptr);

    ray scattered = ray(rec.p,p.generate(),r.time(),r.wavelength());
    auto pdf_val = p.value(scattered.direction());

    return emitted[getIndex(r.wavelength())]
           + srec.attenuation[getIndex(r.wavelength())] * rec.mat_ptr->scattering_pdf(r, rec, scattered)
             * ray_spectrum(scattered, background, world, lights, depth-1) / pdf_val;
}


int main() {

    const auto aspect_ratio = 1.0;
    const int image_width = 500;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int sample_per_pixel = 3;
    const int max_depth = 10;
    const spectrum background = const_spectrum(0);

    const int frameNum = 5;

    for (int frame = 0; frame < frameNum; ++frame) {
        double t_ratio = (double) frame / (double) frameNum;
        //render_target
        std::unique_ptr<RGBA[][image_width]> rgba(new(std::nothrow) RGBA[image_height][image_width]);
        if (!rgba) return -1;

//        //normal_buffer
//        std::unique_ptr<RGBA[][image_width]> normals(new(std::nothrow) RGBA[image_height][image_width]);
//        if (!normals) return -1;
//        //objectId_buffer
//        std::unique_ptr<RGBA[][image_width]> objectId(new(std::nothrow) RGBA[image_height][image_width]);
//        if (!objectId) return -1;

        camera cam;
        hittable_list world = cornell_box(cam, aspect_ratio,t_ratio);
        auto lights = make_shared<hittable_list>();
        lights->add(make_shared<xz_rect>(213, 343, 227, 332, 554, nullptr));
        lights->add(make_shared<sphere>(point3(190, 90, 190), 90, nullptr));

        clock_t start, stop;
        start = clock();

        for (int j = image_height - 1; j >= 0; --j) {
            std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
            #pragma omp parallel for
            for (int i = 0; i < image_width; ++i) {
                color pixel_color(0, 0, 0);
                double pixel_Luminance;
                spectrum pixel_spectrum;
                #pragma omp parallel for
                for (int s = 0; s < sample_per_pixel; ++s) {
                    auto u = (i + random_double()) / (image_width - 1);
                    auto v = (j + random_double()) / (image_height - 1);
                    #pragma omp parallel for
                    for (int k = getIndex(400); k <= getIndex(700); ++k) {
                        ray r = cam.get_ray(u, v, getWaveLength(k));
                        pixel_spectrum[k] += ray_spectrum(r, background, world, lights, max_depth);
                    }
                }
                spectrum pixel_spectrum2 = pixel_spectrum / sample_per_pixel;
                pixel_color = xyz_to_srgb(spectrum_to_xyz(pixel_spectrum2));
                pixel_Luminance = spectrum_to_Luminance(pixel_spectrum2);
                write_color(rgba[image_height - 1 - j][i], pixel_color, pixel_Luminance);
            }
        }
        stop = clock();
        double timer_seconds = ((double) (stop - start)) / CLOCKS_PER_SEC;
        std::cerr << "\nDone.\n";
        std::cerr << "took " << timer_seconds << " seconds.\n";
        stbi_write_png(dayName(frame).c_str(), static_cast<int>(image_width), static_cast<int>(image_height),
                       static_cast<int>(sizeof(RGBA)), rgba.get(), 0);
    }
}
