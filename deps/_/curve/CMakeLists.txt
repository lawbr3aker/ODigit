cmake_minimum_required(VERSION 3.26)
project(curve VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB LIBRARY_HEADERS src/*.h)
file(GLOB LIBRARY_SOURCES src/*.cpp)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_library(${PROJECT_NAME} ${LIBRARY_HEADERS} ${LIBRARY_SOURCES})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_11)
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
