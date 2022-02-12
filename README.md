# Spectral_Renderer
レイに波長情報を持たせたCPUベースのレイトレーサーです。

# Features
分光を見ることができます。

Objファイルを読み込めます。（マテリアルは非対応）

[RayTracing in One Weekend Series](https://raytracing.github.io/books/RayTracingInOneWeekend.html)の発展形です。

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
