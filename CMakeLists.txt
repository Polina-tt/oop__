cmake_minimum_required(VERSION 3.20)

project(
    CampusAssetManager
    VERSION 1.0
    LANGUAGES CXX
)

add_executable(
    campus_assets
    src/main.cpp
    src/equipment_record.cpp
)

target_include_directories(
    campus_assets
    PRIVATE
    include
)

target_compile_features(
    campus_assets
    PRIVATE
    cxx_std_20
)

set_target_properties(
    campus_assets
    PROPERTIES
    CXX_EXTENSIONS OFF
)

if(MSVC)
    target_compile_options(
        campus_assets
        PRIVATE
        /W4
    )
else()
    target_compile_options(
        campus_assets
        PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()