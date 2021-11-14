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
#include <string.h>

color ray_color(const ray& r, const color& background, int depth) {
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}

spectrum ray_spectrum(const ray& r, int depth){
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5*(unit_direction.y() + 1.0);
    spectrum ray;
    for (int i = 0; i < SAMPLE_NUM; ++i) {
        ray.e[i] = d65_data[i] * (t*cie1931_z_data[i] + (1.0 - t)*cie1931_x_data[i])*10.f;
    }
    return ray;
}


int main() {

    const auto aspect_ratio = 1.0;
    const int image_width = 500;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int sample_per_pixel = 500;
    const int max_depth = 50;
    const color background(0,0,0);

    std::unique_ptr<RGBA[][image_width]> rgba(new(std::nothrow) RGBA[image_height][image_width]);
    if (!rgba) return -1;

    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    auto origin = point3(0, 0, 0);
    auto horizontal = vec3(viewport_width, 0, 0);
    auto vertical = vec3(0, viewport_height, 0);
    auto lower_left_corner =
            origin - horizontal/2 - vertical/2 - vec3(0, 0, focal_length);

    clock_t start, stop;
    start = clock();

    for(int j = image_height -1 ; j >= 0; --j){
        std::cerr << "\rScanlines remaining: "  << j << ' ' << std::flush;
#pragma omp parallel for
        for(int i = 0; i < image_width; ++i){
            color pixel_color(0,0,0);
            spectrum pixel_spectrum;
//#pragma omp parallel for
//            for(int s = 0; s < sample_per_pixel; ++s){
                auto u = (i + random_double())/(image_width-1);
                auto v = (j + random_double())/(image_height-1);
                ray r(origin, lower_left_corner + u*horizontal + v*vertical - origin);
                pixel_spectrum = ray_spectrum(r,max_depth);
//            }
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
