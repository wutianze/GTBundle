cmake_minimum_required(VERSION 3.12.4)
SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

if(NOT CMAKE_VERSION VERSION_LESS 3.0)
    cmake_policy(SET CMP0048 NEW)
endif()

project(GTBundle)

#include dir
include_directories("include")

# Find requirements
if(NOT fastcdr_FOUND)
    find_package(fastcdr REQUIRED)
endif()
#find_package(Pistache 0.0.2 REQUIRED)

if(NOT fastrtps_FOUND)
    find_package(fastrtps REQUIRED)
endif()

# Set C++11
include(CheckCXXCompilerFlag)
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANG OR
        CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    check_cxx_compiler_flag(-std=c++14 SUPPORTS_CXX14)
    if(SUPPORTS_CXX14)
        add_compile_options(-std=c++14)
    else()
        message(FATAL_ERROR "Compiler doesn't support C++11")
    endif()
endif()
message(STATUS "Configuring HelloWorld publisher/subscriber example...")
file(GLOB SOURCES_CXX "src/*.cxx")
file(GLOB SOURCES_CPP "src/*.cpp")
add_executable(bundle src/main.cpp ${SOURCES_CXX} ${SOURCES_CPP})
target_link_libraries(bundle fastrtps fastcdr)
