cmake_minimum_required(VERSION 3.14)

project(ODigit LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS -O3)

include_directories(deps/include)

set(Qt5_DIR "F:\\Qt\\5.15.2\\mingw81_64\\lib\\cmake\\Qt5")
find_package(Qt5 COMPONENTS Core Quick Widgets REQUIRED)

set(PROJECT_SOURCES
    src/qml.qrc
    src/main.cpp

    src/core/macros.h

    src/core/utils/polygon.hpp
    src/core/utils/polygon.cpp
    src/core/utils/image.hpp
    src/core/utils/image.cpp
    src/core/utils/dxf.cpp
    src/core/utils/dxf.hpp
    src/core/utils/algebra.cpp
    src/core/utils/algebra.hpp
    src/core/utils/hash.cpp
    src/core/utils/hash.hpp

    src/core/gui/scripts/config.hpp
    src/core/gui/scripts/config.cpp
    src/core/gui/scripts/process.hpp
    src/core/gui/scripts/process.cpp
    src/core/gui/scripts/registration.cpp
    src/core/gui/scripts/registration.hpp

    src/core/gui/components/editor.hpp
    src/core/gui/components/editor.cpp
    src/core/gui/components/ruler.hpp
    src/core/gui/components/ruler.cpp
)

if (${CMAKE_BUILD_TYPE} STREQUAL Release)
    add_executable(${PROJECT_NAME}
        WIN32
        ${PROJECT_SOURCES}
    )
else()
    add_executable(${PROJECT_NAME}
        ${PROJECT_SOURCES}
    )
endif()

file(GLOB
    PROJECT_DLLS ${PROJECT_SOURCE_DIR}/deps/bin/*.dll
)
file(GLOB
    PROJECT_LIBS ${PROJECT_SOURCE_DIR}/deps/bin/*.a
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Qt5::Core
        Qt5::Quick
        Qt5::Widgets
        ${PROJECT_DLLS}
        ${PROJECT_LIBS}
)
