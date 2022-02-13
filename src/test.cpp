//
// Created by Vimii on 2021/12/07.
//
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
#include "../Eigen/Dense"
#include <typeinfo.h>

using Eigen::MatrixXd;

int main(){
    auto light = make_shared<diffuse_light>(make_shared<solid_color>(D65*100));
    auto glass = make_shared<dielectric>(1.5, 1);
    if(typeid(light) == typeid(glass)) std::cout << "uouo" << std::endl;
    return 0;
}
