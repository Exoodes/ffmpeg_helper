cmake_minimum_required(VERSION 3.15)
project(FFMPEGWrapper CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(fmt REQUIRED)
find_package(FFmpeg REQUIRED)
find_package(CLI11 REQUIRED)

add_executable(ffmpeg_wrapper src/main.cpp)

target_link_libraries(ffmpeg_wrapper PRIVATE 
    fmt::fmt 
    ffmpeg::ffmpeg
    CLI11::CLI11
)