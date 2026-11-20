cmake_minimum_required(VERSION 3.16)
project(lab07_templates LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(lab07 main.cpp)
target_compile_options(lab07 PRIVATE -Wall -Wextra -Wpedantic)