cmake_minimum_required(VERSION 3.19)
project(Spectral_RayTracing)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-fopenmp") #OpenMP使うならオプションをset

add_executable(Spectral_RayTracing src/main.cpp src/spectrum_Data.h src/triangle.h src/loadObj.h src/ObjModel.h)
#add_executable(Spectral_RayTracing src/test.cpp)