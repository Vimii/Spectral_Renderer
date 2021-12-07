//
// Created by Vimii on 2021/11/01.
//
#pragma once
#ifndef SPECTRAL_RAYTRACING_UTIL_H
#define SPECTRAL_RAYTRACING_UTIL_H
#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180;
}

inline int random_int(int min, int max) {
    // {min, min+1, ..., max} から整数をランダムに返す
    return min + rand() % (max - min + 1);
}

inline double random_double() {
    // [0,1) の実数乱数を返す
    return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
    // [min,max) の実数乱数を返す
    return min + (max-min)*random_double();
}

//inline double random_double_cpp() {
//    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
//    static std::mt19937 generator;
//    return distribution(generator);
//}

inline double clamp(double x, double min, double max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

//共通ヘッダー
#include "vec3.h"
#include "ray.h"
#include "color.h"

#endif //SPECTRAL_RAYTRACING_UTIL_H
