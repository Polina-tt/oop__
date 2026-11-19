cmake_minimum_required(VERSION 3.20)

project(
    CampusAssetManager
    VERSION 1.0
    LANGUAGES CXX
)

option(ENABLE_SANITIZERS "Enable AddressSanitizer and UndefinedBehaviorSanitizer" OFF)

add_library(
    campus_core
    STATIC
    src/equipment_record.cpp
    src/TimeSlot.cpp
)

target_include_directories(
    campus_core
    PUBLIC
    include
)

target_compile_features(
    campus_core
    PUBLIC
    cxx_std_20
)

set_target_properties(
    campus_core
    PROPERTIES
    CXX_EXTENSIONS OFF
)

if(MSVC)
    target_compile_options(campus_core PRIVATE /W4)
else()
    target_compile_options(campus_core PRIVATE -Wall -Wextra -Wpedantic)
endif()

if(ENABLE_SANITIZERS AND NOT MSVC)
    target_compile_options(
        campus_core
        PUBLIC
        -fsanitize=address,undefined
        -fno-omit-frame-pointer
        -g
    )
    target_link_options(
        campus_core
        PUBLIC
        -fsanitize=address,undefined
        -fno-omit-frame-pointer
        -g
    )
endif()

add_executable(
    campus_assets
    src/main.cpp
)

target_link_libraries(
    campus_assets
    PRIVATE
    campus_core
)

add_executable(
    experiment
    src/experiment.cpp
)

target_compile_features(
    experiment
    PRIVATE
    cxx_std_20
)

if(ENABLE_SANITIZERS AND NOT MSVC)
    target_compile_options(
        experiment
        PRIVATE
        -fsanitize=address,undefined
        -fno-omit-frame-pointer
        -g
    )
    target_link_options(
        experiment
        PRIVATE
        -fsanitize=address,undefined
        -fno-omit-frame-pointer
        -g
    )
endif()

enable_testing()

include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.7.1
)
FetchContent_MakeAvailable(Catch2)
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(Catch)

add_executable(
    unit_tests
    tests/domain_tests.cpp
    tests/coordinator_tests.cpp
)

target_link_libraries(
    unit_tests
    PRIVATE
    campus_core
    Catch2::Catch2WithMain
)

catch_discover_tests(unit_tests)