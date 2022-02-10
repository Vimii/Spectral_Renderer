//
// Created by Vimii on 2021/05/15.
//

#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

#include <iostream>

struct RGBA {
    unsigned char r, g, b, a; //赤, 緑, 青, 透過
    RGBA() = default;
    constexpr RGBA(const unsigned char r_, const unsigned char g_, const unsigned char b_, const unsigned char a_) :r(r_), g(g_), b(b_), a(a_) {}
};

color xyz_to_srgb(const color &xyz) {
    return color(
            3.240479f * xyz.x() -1.537150f * xyz.y() - 0.498535f * xyz.z(),
            -0.969256f * xyz.x() + 1.875991f * xyz.y() + 0.041556f * xyz.z(),
            0.055648f * xyz.x() -0.204043f * xyz.y() + 1.057311f * xyz.z()
            );
}

color srgb_to_xyz(const color &rgb){
    return color(
            0.412453f * rgb.x() + 0.357580f * rgb.x() + 0.180423f * rgb.x(),
            0.212671f * rgb.y() + 0.715160f * rgb.y() + 0.072169f * rgb.y(),
            0.019334f * rgb.z() + 0.119193f * rgb.z() + 0.950227f * rgb.z()
    );
}

color xyz_to_rgb(const color &xyz) {
    return color(
            0.418466f * xyz.x() -0.158661f * xyz.x() - 0.082835f * xyz.x(),
            -0.091169f * xyz.y() + 0.252431f * xyz.y() + 0.015707f * xyz.y(),
            0.000921f * xyz.z() -0.002550f * xyz.z() + 0.178599f * xyz.z()
    );
}


void write_color(struct RGBA &out , color pixel_color,double pixel_Luminance) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();
    auto a = 255;

    if (r != r) r = 0.0;
    if (g != g) g = 0.0;
    if (b != b) b = 0.0;

    //色に輝度をかけて、gamma = 2.0 のガンマ補正を行う。
    r = sqrt(pixel_Luminance * r);
    g = sqrt(pixel_Luminance * g);
    b = sqrt(pixel_Luminance * b);

    out.r = static_cast<int>(256 * clamp(r, 0.0, 0.999));
    out.g = static_cast<int>(256 * clamp(g, 0.0, 0.999));
    out.b = static_cast<int>(256 * clamp(b, 0.0, 0.999));
    out.a = a;
}

#endif //COLOR_H
