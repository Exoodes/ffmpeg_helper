cmake_minimum_required(VERSION 3.15)
project(FFMPEGWrapper CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(fmt REQUIRED)
find_package(FFmpeg REQUIRED)
find_package(CLI11 REQUIRED)

include(FetchContent)
FetchContent_Declare(
    matplotplusplus
    GIT_REPOSITORY https://github.com/alandefreitas/matplotplusplus
    GIT_TAG        v1.2.2
)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(WITH_INTERNAL_CIMG OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(matplotplusplus)

set(MEDIA_SOURCES 
    src/media/MediaAnalyzer.cpp
    src/media/MediaAnalyzer.h
    src/media/Utility.cpp
    src/media/Utility.h
    src/media/SmartPointers.h
    src/media/FFmpegHeaders.h
    src/media/MediaStructs.hpp
)

set(CLI_SOURCES 
    src/cli/Parser.cpp
    src/cli/Parser.h
)

set(COMMAND_SOURCES 
    src/commands/ProbeCommand.cpp
    src/commands/ProbeCommand.h
    src/commands/CastCommand.cpp
    src/commands/CastCommand.h
    src/commands/BitrateCommand.cpp
    src/commands/BitrateCommand.h
    src/commands/ICommand.h
)

set(SERVICE_SOURCES
    src/services/BitratePlotter.cpp
    src/services/BitratePlotter.hpp
)

set(CORE_SOURCES 
    src/main.cpp
)

add_executable(ffmpeg_wrapper 
    ${CORE_SOURCES}
    ${CLI_SOURCES}
    ${COMMAND_SOURCES}
    ${MEDIA_SOURCES}
    ${SERVICE_SOURCES}
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
    matplot
)