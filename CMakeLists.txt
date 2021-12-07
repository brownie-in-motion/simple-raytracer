cmake_minimum_required(VERSION 3.8)

project(raytracer)

add_executable(raytracer
    src/main.cpp
)

target_compile_features(raytracer PUBLIC cxx_std_17)
