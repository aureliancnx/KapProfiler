cmake_minimum_required(VERSION 3.16)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "Vcpkg toolchain file")

if(${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_BINARY_DIR} AND NOT WIN32)
    message(FATAL_ERROR
            "You can not use CMake to build from the root of it's source tree!\n"
            "Remove the CMakeCache.txt file from this directory, then create a separate directory\n"
            "(either below this directory or elsewhere), and then re-run CMake from there.")
endif(${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_BINARY_DIR} AND NOT WIN32)

project(KapProfiler LANGUAGES CXX VERSION 1.0.1)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake" ${CMAKE_MODULE_PATH})

set(CMAKE_CXX_STANDARD 20)

# use valgrind programm
option(VALGRIND "Use -g on project compile" OFF)

# RayLib version
set(raylib_VERSION 4.0.0)
find_package(raylib REQUIRED)
message(STATUS "RayLib version: ${raylib_VERSION}")

set(ZLIB_USE_STATIC_LIBS ON)
find_package(ZLIB REQUIRED)

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED COMPONENTS iostreams)

set(IMGUI_USE_STATIC_LIBS ON)
find_package(imgui REQUIRED)

set(GLFW3_USE_STATIC_LIBS ON)
find_package(glfw3 CONFIG REQUIRED)

file(GLOB_RECURSE SOURCES src/*.cpp)

add_executable(KapProfiler ${SOURCES} ${HEADERS})

include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(KapProfiler raylib)
target_link_libraries(KapProfiler Boost::iostreams)
target_link_libraries(KapProfiler glfw)

target_link_libraries(KapProfiler ZLIB::ZLIB)
target_link_libraries(KapProfiler imgui::imgui)

# Includes
include_directories(.)

if (VALGRIND)
    target_compile_options(KapProfiler PRIVATE -g)
endif()
IF (NOT WIN32)
    target_compile_options(KapProfiler PRIVATE -O2)
ENDIF()
target_compile_options(KapProfiler PRIVATE -pthread)
