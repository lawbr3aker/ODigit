project(curve-decompress)
cmake_minimum_required(VERSION 2.8)

find_package(OpenCV REQUIRED)
find_package(DGtal REQUIRED)
find_package(Boost REQUIRED COMPONENTS program_options)

link_directories(${OpenCV_LIBRARY_DIRS} ${DGTAL_LIBRARY_DIRS} ${Boost_LIBRARY_DIRS})

include_directories(${OpenCV_INCLUDE_DIRS} ${DGTAL_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})

add_definitions(-g)

aux_source_directory(./src SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})

target_link_libraries(${PROJECT_NAME}
        ${OpenCV_LIBS}
        ${DGTAL_LIBRARIES}
        ${Boost_LIBRAIRIES} ${Boost_PROGRAM_OPTIONS_LIBRARY})