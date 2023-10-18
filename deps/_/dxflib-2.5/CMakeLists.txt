cmake_minimum_required(VERSION 3.26)
project(dxflib VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB LIBRARY_HEADERS src/*.h)
file(GLOB LIBRARY_SOURCES src/*.cpp)

add_library(${PROJECT_NAME} ${LIBRARY_HEADERS} ${LIBRARY_SOURCES})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_11)

