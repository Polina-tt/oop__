cmake_minimum_required(VERSION 3.15)
project(EquipmentManagement CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Обов'язкове увімкнення генерації конфігурації для Clang-Tidy
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(equipment_app
    src/main.cpp
    src/equipment_item.cpp
)

target_include_directories(equipment_app PRIVATE include)