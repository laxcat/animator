cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_STANDARD 17)
project(AnimatorTests VERSION 1.0 LANGUAGES CXX)
add_executable(animator_tests 
    animator_tests.cpp
    Animator.cpp
)
