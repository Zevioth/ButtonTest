cmake_minimum_required(VERSION 3.22)

project(ButtonTest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(MOD_ID "buttontest" CACHE STRING "LeviLauncher mod package directory name")
set(MOD_NAME "Button Test" CACHE STRING "LeviLauncher mod display name")
set(MOD_AUTHOR "SkyeMoz" CACHE STRING "LeviLauncher mod author")
set(MOD_VERSION "1.0.0" CACHE STRING "LeviLauncher mod version")
set(MOD_LIBRARY_NAME "buttontest" CACHE STRING "Native library name")
set(MOD_MINECRAFT_VERSIONS "[\"1.26.*\"]" CACHE STRING "Minecraft versions")
set(MOD_ICON "" CACHE STRING "Optional icon")

include(FetchContent)

FetchContent_Declare(
    preloader_android
    GIT_REPOSITORY https://github.com/LiteLDev/LeviLaunchroid.git
    GIT_TAG main
)

FetchContent_MakeAvailable(preloader_android)

find_package(nlohmann_json REQUIRED)
find_package(fmt REQUIRED)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/manifest.json.in"
    "${CMAKE_CURRENT_BINARY_DIR}/manifest.json"
    @ONLY
)

file(GLOB_RECURSE MOD_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_library(${MOD_LIBRARY_NAME} SHARED
    ${MOD_SOURCES}
)

target_include_directories(${MOD_LIBRARY_NAME}
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
        "${preloader_android_SOURCE_DIR}/include"
)

target_link_libraries(${MOD_LIBRARY_NAME}
    PRIVATE
        preloader_android
        nlohmann_json::nlohmann_json
        fmt::fmt
)

set_target_properties(${MOD_LIBRARY_NAME} PROPERTIES
    OUTPUT_NAME "${MOD_LIBRARY_NAME}"
    PREFIX "lib"
)

add_executable(levi_config_generator
    src/config_generator.cpp
    src/mod/Config.cpp
)

target_include_directories(levi_config_generator
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
        "${preloader_android_SOURCE_DIR}/include"
        "$ENV{JAVA_HOME}/include"
        "$ENV{JAVA_HOME}/include/linux"
)

target_link_libraries(levi_config_generator
    PRIVATE
        nlohmann_json::nlohmann_json
        fmt::fmt
)

target_compile_definitions(
    levi_config_generator
    PRIVATE
        PL_CONFIG_NO_RUNTIME
)

install(
    TARGETS ${MOD_LIBRARY_NAME}
    LIBRARY DESTINATION .
)

install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/manifest.json"
    DESTINATION .
)
