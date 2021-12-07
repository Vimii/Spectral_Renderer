//
// Created by Vimii on 2021/11/01.
//

#ifndef SPECTRAL_RAYTRACING_SPECTRUM_H
#define SPECTRAL_RAYTRACING_SPECTRUM_H

#if !defined(MTS_WAVELENGTH_MIN)
#  define MTS_WAVELENGTH_MIN 360.f
#endif

#if !defined(MTS_WAVELENGTH_MAX)
#  define MTS_WAVELENGTH_MAX 830.f
#endif

#if !defined(SAMPLE_NUM)
#  define SAMPLE_NUM 95
#endif

#include <algorithm>
#include "d65.h"
#include "color.h"
#include <string>
#include <map>
#include <sstream>
#include <vector>

enum SPEC_TYPE{
    _EMPTY,
    _RED,
    _GREEN,
    _BLUE,
    _D65
};

class spectrum{
public:
    spectrum() : e{0}{}
    spectrum(const float spec[]){
        for (int i = 0; i < SAMPLE_NUM; ++i) {
            e[i] = spec[i];
        }
    }

    float operator[](int i) const { return e[i];}
    float& operator[](int i){ return e[i];}

    spectrum& operator+=(const spectrum &s){
        for (int i = 0; i < SAMPLE_NUM; ++i)
            e[i] += s.e[i];
        return *this;
    }

    spectrum& operator*=(const double t) {
        for (float v : e)
            v *= t;
        return *this;
    }

    spectrum& operator/=(const double t) {
        return *this *= 1/t;
    }

    float max_intensity() const{
        return *std::max_element(e,e+SAMPLE_NUM);
    }

public:
    float e[SAMPLE_NUM];
};

//util

inline spectrum operator+(const spectrum &u, const spectrum &v){
    spectrum result;
    for (int i = 0; i < SAMPLE_NUM; ++i) {
        result.e[i] = u.e[i] + v.e[i];
    }
    return result;
}

inline spectrum operator-(const spectrum &u, const spectrum &v){
    spectrum result;
    for (int i = 0; i < SAMPLE_NUM; ++i) {
        result.e[i] = u.e[i] - v.e[i];
    }
    return result;
}

inline spectrum operator*(const spectrum &u, const spectrum &v){
    spectrum result;
    for (int i = 0; i < SAMPLE_NUM; ++i) {
        result.e[i] = u.e[i] * v.e[i];
    }
    return result;
}

inline spectrum operator*(double t, const spectrum &v){
    spectrum result;
    for (int i = 0; i < SAMPLE_NUM; ++i) {
        result.e[i] = t * v.e[i];
    }
    return result;
}


inline spectrum operator*(const spectrum &v, double t){
    spectrum result;
    for (int i = 0; i < SAMPLE_NUM; ++i) {
        result.e[i] = t * v.e[i];
    }
    return result;
}

inline spectrum operator/(const spectrum &v, double t){
    spectrum result;
    for (int i = 0; i < SAMPLE_NUM; ++i) {
        result.e[i] = (1/t) * v.e[i];
    }
    return result;
}

inline spectrum ratio_spectrum(spectrum v){
    return v / v.max_intensity();
}

inline spectrum const_spectrum(float v){
    spectrum result;
    for (int i = 0; i < SAMPLE_NUM; ++i) {
        result.e[i] = v;
    }
    return result;
}

/*CIEのxyz等色関数*/
using Float = float;

static const Float cie1931_tbl[SAMPLE_NUM * 3] = {
        Float(0.000129900000), Float(0.000232100000), Float(0.000414900000), Float(0.000741600000),
        Float(0.001368000000), Float(0.002236000000), Float(0.004243000000), Float(0.007650000000),
        Float(0.014310000000), Float(0.023190000000), Float(0.043510000000), Float(0.077630000000),
        Float(0.134380000000), Float(0.214770000000), Float(0.283900000000), Float(0.328500000000),
        Float(0.348280000000), Float(0.348060000000), Float(0.336200000000), Float(0.318700000000),
        Float(0.290800000000), Float(0.251100000000), Float(0.195360000000), Float(0.142100000000),
        Float(0.095640000000), Float(0.057950010000), Float(0.032010000000), Float(0.014700000000),
        Float(0.004900000000), Float(0.002400000000), Float(0.009300000000), Float(0.029100000000),
        Float(0.063270000000), Float(0.109600000000), Float(0.165500000000), Float(0.225749900000),
        Float(0.290400000000), Float(0.359700000000), Float(0.433449900000), Float(0.512050100000),
        Float(0.594500000000), Float(0.678400000000), Float(0.762100000000), Float(0.842500000000),
        Float(0.916300000000), Float(0.978600000000), Float(1.026300000000), Float(1.056700000000),
        Float(1.062200000000), Float(1.045600000000), Float(1.002600000000), Float(0.938400000000),
        Float(0.854449900000), Float(0.751400000000), Float(0.642400000000), Float(0.541900000000),
        Float(0.447900000000), Float(0.360800000000), Float(0.283500000000), Float(0.218700000000),
        Float(0.164900000000), Float(0.121200000000), Float(0.087400000000), Float(0.063600000000),
        Float(0.046770000000), Float(0.032900000000), Float(0.022700000000), Float(0.015840000000),
        Float(0.011359160000), Float(0.008110916000), Float(0.005790346000), Float(0.004109457000),
        Float(0.002899327000), Float(0.002049190000), Float(0.001439971000), Float(0.000999949300),
        Float(0.000690078600), Float(0.000476021300), Float(0.000332301100), Float(0.000234826100),
        Float(0.000166150500), Float(0.000117413000), Float(0.000083075270), Float(0.000058706520),
        Float(0.000041509940), Float(0.000029353260), Float(0.000020673830), Float(0.000014559770),
        Float(0.000010253980), Float(0.000007221456), Float(0.000005085868), Float(0.000003581652),
        Float(0.000002522525), Float(0.000001776509), Float(0.000001251141),

        Float(0.000003917000), Float(0.000006965000), Float(0.000012390000), Float(0.000022020000),
        Float(0.000039000000), Float(0.000064000000), Float(0.000120000000), Float(0.000217000000),
        Float(0.000396000000), Float(0.000640000000), Float(0.001210000000), Float(0.002180000000),
        Float(0.004000000000), Float(0.007300000000), Float(0.011600000000), Float(0.016840000000),
        Float(0.023000000000), Float(0.029800000000), Float(0.038000000000), Float(0.048000000000),
        Float(0.060000000000), Float(0.073900000000), Float(0.090980000000), Float(0.112600000000),
        Float(0.139020000000), Float(0.169300000000), Float(0.208020000000), Float(0.258600000000),
        Float(0.323000000000), Float(0.407300000000), Float(0.503000000000), Float(0.608200000000),
        Float(0.710000000000), Float(0.793200000000), Float(0.862000000000), Float(0.914850100000),
        Float(0.954000000000), Float(0.980300000000), Float(0.994950100000), Float(1.000000000000),
        Float(0.995000000000), Float(0.978600000000), Float(0.952000000000), Float(0.915400000000),
        Float(0.870000000000), Float(0.816300000000), Float(0.757000000000), Float(0.694900000000),
        Float(0.631000000000), Float(0.566800000000), Float(0.503000000000), Float(0.441200000000),
        Float(0.381000000000), Float(0.321000000000), Float(0.265000000000), Float(0.217000000000),
        Float(0.175000000000), Float(0.138200000000), Float(0.107000000000), Float(0.081600000000),
        Float(0.061000000000), Float(0.044580000000), Float(0.032000000000), Float(0.023200000000),
        Float(0.017000000000), Float(0.011920000000), Float(0.008210000000), Float(0.005723000000),
        Float(0.004102000000), Float(0.002929000000), Float(0.002091000000), Float(0.001484000000),
        Float(0.001047000000), Float(0.000740000000), Float(0.000520000000), Float(0.000361100000),
        Float(0.000249200000), Float(0.000171900000), Float(0.000120000000), Float(0.000084800000),
        Float(0.000060000000), Float(0.000042400000), Float(0.000030000000), Float(0.000021200000),
        Float(0.000014990000), Float(0.000010600000), Float(0.000007465700), Float(0.000005257800),
        Float(0.000003702900), Float(0.000002607800), Float(0.000001836600), Float(0.000001293400),
        Float(0.000000910930), Float(0.000000641530), Float(0.000000451810),

        Float(0.000606100000), Float(0.001086000000), Float(0.001946000000), Float(0.003486000000),
        Float(0.006450001000), Float(0.010549990000), Float(0.020050010000), Float(0.036210000000),
        Float(0.067850010000), Float(0.110200000000), Float(0.207400000000), Float(0.371300000000),
        Float(0.645600000000), Float(1.039050100000), Float(1.385600000000), Float(1.622960000000),
        Float(1.747060000000), Float(1.782600000000), Float(1.772110000000), Float(1.744100000000),
        Float(1.669200000000), Float(1.528100000000), Float(1.287640000000), Float(1.041900000000),
        Float(0.812950100000), Float(0.616200000000), Float(0.465180000000), Float(0.353300000000),
        Float(0.272000000000), Float(0.212300000000), Float(0.158200000000), Float(0.111700000000),
        Float(0.078249990000), Float(0.057250010000), Float(0.042160000000), Float(0.029840000000),
        Float(0.020300000000), Float(0.013400000000), Float(0.008749999000), Float(0.005749999000),
        Float(0.003900000000), Float(0.002749999000), Float(0.002100000000), Float(0.001800000000),
        Float(0.001650001000), Float(0.001400000000), Float(0.001100000000), Float(0.001000000000),
        Float(0.000800000000), Float(0.000600000000), Float(0.000340000000), Float(0.000240000000),
        Float(0.000190000000), Float(0.000100000000), Float(0.000049999990), Float(0.000030000000),
        Float(0.000020000000), Float(0.000010000000), Float(0.000000000000), Float(0.000000000000),
        Float(0.000000000000), Float(0.000000000000), Float(0.000000000000), Float(0.000000000000),
        Float(0.000000000000), Float(0.000000000000), Float(0.000000000000), Float(0.000000000000),
        Float(0.000000000000), Float(0.000000000000), Float(0.000000000000), Float(0.000000000000),
        Float(0.000000000000), Float(0.000000000000), Float(0.000000000000), Float(0.000000000000),
        Float(0.000000000000), Float(0.000000000000), Float(0.000000000000), Float(0.000000000000),
        Float(0.000000000000), Float(0.000000000000), Float(0.000000000000), Float(0.000000000000),
        Float(0.000000000000), Float(0.000000000000), Float(0.000000000000), Float(0.000000000000),
        Float(0.000000000000), Float(0.000000000000), Float(0.000000000000), Float(0.000000000000),
        Float(0.000000000000), Float(0.000000000000), Float(0.000000000000)
};

const Float *cie1931_x_data = cie1931_tbl;
const Float *cie1931_y_data = cie1931_tbl + SAMPLE_NUM;
const Float *cie1931_z_data = cie1931_tbl + SAMPLE_NUM * 2;

spectrum spectrum_type(SPEC_TYPE st){
    spectrum result;
    switch (st) {
        case _EMPTY:
            break;
        case _RED:
            for (int i = 0; i < SAMPLE_NUM; ++i){
                color r = srgb_to_xyz(color(xyz_to_srgb(color(cie1931_x_data[i],cie1931_y_data[i],cie1931_z_data[i])).x(),0,0));
                result.e[i] = r.x() + r.y() + r.z();
            }
            break;
        case _GREEN:
            for (int i = 0; i < SAMPLE_NUM; ++i) {
                color g = srgb_to_xyz(color(0, xyz_to_srgb(color(cie1931_x_data[i], cie1931_y_data[i], cie1931_z_data[i])).y(),0));
                result.e[i] = g.x() + g.y() + g.z();
            }
            break;
        case _BLUE:
            for (int i = 0; i < SAMPLE_NUM; ++i) {
                color b = srgb_to_xyz(color(0, 0, xyz_to_srgb(color(cie1931_x_data[i], cie1931_y_data[i], cie1931_z_data[i])).z()));
                result.e[i] = b.x() + b.y() + b.z();
            }
            break;
        case _D65:
            for (int i = 0; i < SAMPLE_NUM; ++i)
                result.e[i] = d65_data[i];
            break;
    }
    return result;
}

static const spectrum RED = spectrum_type(_RED);
static const spectrum GREEN = spectrum_type(_GREEN);
static const spectrum BLUE = spectrum_type(_BLUE);
static const spectrum D65 = spectrum_type(_D65);

inline const int getWaveLength(int index){
    return (int)(MTS_WAVELENGTH_MIN + 5 * index);
}

inline const int getIndex(int wavelength){
    return (int)((wavelength - MTS_WAVELENGTH_MIN) / 5);
}

inline const float xyz_k(){
    return 21.371408462524414;
}

inline color spectrum_to_xyz(spectrum sp){
    color xyz(0.f,0.f,0.f);

    for(int i=0;i<SAMPLE_NUM;i++)
        xyz += color(
                sp.e[i] * cie1931_x_data[i],
                sp.e[i] * cie1931_y_data[i],
                sp.e[i] * cie1931_z_data[i]
                );

    xyz = color(xyz.x() / xyz_k(),
                xyz.y() / xyz_k(),
                xyz.z() / xyz_k()
                );

    return xyz;
}

using namespace std;

vector<string> split_naive(const string &s, char delim) {
    vector<string> elems;
    string item;
    for (char ch: s) {
        if (ch == delim) {
            if (!item.empty())
                elems.push_back(item);
            item.clear();
        }
        else {
            item += ch;
        }
    }
    if (!item.empty())
        elems.push_back(item);
    return elems;
}

spectrum stospec(const string &s){
    auto elems = split_naive(s,' ');
    map<int, float> specpair;
    float spec[SAMPLE_NUM];
    int wl = MTS_WAVELENGTH_MIN;
    for (int i = 0; i < SAMPLE_NUM; ++i) {
        spec[i] = 0.0f;
    }
    for(auto uo : elems){
        if(uo.back() == ',')
            uo.erase(uo.length()-1);
//        std::cout << uo << std::endl;
        auto hoge = split_naive(uo,':');
        int wavlen = stoi(hoge.front());
        float ratio = stof(hoge.back());
//        std::cout << wavlen << "@" << ratio << std::endl;
        specpair.insert(make_pair(wavlen,ratio));
    }
    for (int i = 360; i < MTS_WAVELENGTH_MAX; i+=(MTS_WAVELENGTH_MAX-MTS_WAVELENGTH_MIN)/(SAMPLE_NUM-1)) {
//        std::cout << i << "_[" << specpair.upper_bound(i)->first << "," << (specpair.lower_bound(i))->first << std::endl;
        if(specpair.upper_bound(i)->first != specpair.lower_bound(i)->first){
            if(specpair.upper_bound(i)->first < specpair.lower_bound(i)->first){break;}
            spec[getIndex(i)] = specpair.lower_bound(i)->second;
        }
        else if(specpair.upper_bound(i)->first == specpair.upper_bound(0)->first)
        { continue;}
        else {
            float r = (float)(i - (--specpair.upper_bound(i))->first)
                      / ((float)(specpair.upper_bound(i)->first - (--specpair.upper_bound(i))->first));
//            std::cout << r << std::endl;
            spec[getIndex(i)] = (float)(specpair.upper_bound(i)->second) * r + (float)((--specpair.upper_bound(i))->second) * (1.0f - r);

        }
    }
    return spectrum(spec);
}

#endif //SPECTRAL_RAYTRACING_SPECTRUM_H
