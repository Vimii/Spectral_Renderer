cmake_minimum_required(VERSION 3.19)
project(Spectral_RayTracing)

set(CMAKE_CXX_STANDARD 17)

add_executable(Spectral_RayTracing src/main.cpp src/vec3.h src/color.h src/util.h)