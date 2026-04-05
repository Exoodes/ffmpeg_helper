cmake_minimum_required(VERSION 3.15)
project(FFMPEGWrapper CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(fmt REQUIRED)
find_package(FFmpeg REQUIRED)
find_package(CLI11 REQUIRED)

set(CLI_SOURCES 
    src/cli/Parser.cpp
)

set(COMMAND_SOURCES 
    src/commands/ProbeCommand.cpp
    src/commands/CastCommand.cpp
)

set(CORE_SOURCES 
    src/main.cpp
)

add_executable(ffmpeg_wrapper 
    ${CORE_SOURCES}
    ${CLI_SOURCES}
    ${COMMAND_SOURCES}
)

target_include_directories(ffmpeg_wrapper PRIVATE src)

if(MSVC)
    target_compile_options(ffmpeg_wrapper PRIVATE /W4 /permissive-)
else()
    target_compile_options(ffmpeg_wrapper PRIVATE 
    -Wall 
    -Wextra 
    -pedantic
    )
endif()

target_link_libraries(ffmpeg_wrapper PRIVATE 
    fmt::fmt 
    ffmpeg::ffmpeg
    CLI11::CLI11
)