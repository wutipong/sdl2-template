cmake_minimum_required(VERSION 3.17)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake
        CACHE STRING "Vcpkg toolchain file")

# set the project name
project(sdl2-template)

# add the executable
add_executable(sdl2-template "main.cpp")

target_compile_features(sdl2-template PRIVATE cxx_std_20)
set_property(TARGET sdl2-template PROPERTY CXX_STANDARD 20)
set_property(TARGET sdl2-template PROPERTY CXX_STANDARD_REQUIRED ON)

if (MSVC)
    set_property(TARGET sdl2-template PROPERTY WIN32_EXECUTABLE ON)
    target_compile_options(sdl2-template PRIVATE /Zc:__cplusplus)
endif ()

find_package(SDL2 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

target_include_directories(sdl2-template PRIVATE ${STB_INCLUDE_DIRS})

target_link_libraries(sdl2-template PRIVATE
        SDL2::SDL2 SDL2::SDL2main
        imgui::imgui)