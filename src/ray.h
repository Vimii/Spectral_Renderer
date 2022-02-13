//
// Created by Vimii on 2021/05/15.
//
#pragma once
#ifndef RAY_H
#define RAY_H


#include "vec3.h"

class ray {
public:
    ray() {}
    ray(const point3& origin, const vec3& direction, double time = 0.0, const double wavelength = 500.0,bool sep = false)
            : orig(origin), dir(direction), tm(time), wl(wavelength),separated(sep)
    {}

    point3 origin() const  { return orig; }
    vec3 direction() const { return dir; }
    double time() const    { return tm; }
    double wavelength() const { return wl;}

    point3 at(double t) const {
        return orig + t*dir;
    }

public:
    point3 orig;
    vec3 dir;
    double tm;
    double wl;
    bool separated;
};

#endif //RAY_H
