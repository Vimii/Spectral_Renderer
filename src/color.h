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

void write_color(struct RGBA &out , color pixel_color, int samples_per_pixel) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();
    auto a = 255;

    if (r != r) r = 0.0;
    if (g != g) g = 0.0;
    if (b != b) b = 0.0;

    //色の和をサンプル数で割り、gamma = 2.0 のガンマ補正を行う。
    auto scale = 1.0 / samples_per_pixel;
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);

    out.r = static_cast<int>(256 * clamp(r, 0.0, 0.999));
    out.g = static_cast<int>(256 * clamp(g, 0.0, 0.999));
    out.b = static_cast<int>(256 * clamp(b, 0.0, 0.999));
    out.a = a;
}

#endif //COLOR_H
