# Spectral_Renderer
![2_17_15_27_0](https://user-images.githubusercontent.com/61103348/154852896-b9c26aa6-db55-4c91-a4f8-68cf59eac0d0.jpg)
レイに波長情報を持たせたCPUベースのレイトレーサーです。



[RayTracing in One Weekend Series](https://raytracing.github.io/books/RayTracingInOneWeekend.html)の発展形です。
# Features
分光を見ることができます。

Objファイルを読み込めます。（マテリアルは非対応）


## Dependence
  [mingw-w64 x86_64-8.1.0-posix-seh](https://www.mingw-w64.org)
  
  [cmake 3.19](https://cmake.org/download/)
  
## Build

``` terminal
>mkdir build
>cd build
>cmake -G "MinGW Makefiles" ..
>cmake --build .
```
