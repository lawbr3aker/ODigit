cmake_minimum_required(VERSION 3.14)

project(ODigit LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(APPLICATION_TEST "name")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(deps/include)

set(Qt5_DIR "F:\\Qt\\5.15.2\\mingw81_64\\lib\\cmake\\Qt5")
find_package(Qt5 COMPONENTS Core Quick Widgets REQUIRED)

set(PROJECT_SOURCES
    src/main.cpp
    src/qml/qml.qrc
    src/scripts/process.h
    src/scripts/editor.h
    src/scripts/config.h
    src/scripts/translator.h
    src/scripts/ruler.h
)
set(PROJECT_TRANSLATIONS
    src/translations/tr_fa.ts
)

add_executable(${PROJECT_NAME}
    #WIN32
    ${PROJECT_SOURCES}
)

file(GLOB
    PROJECT_LIBS ${PROJECT_SOURCE_DIR}/deps/bin/*.dll
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Qt5::Core
        Qt5::Quick
        Qt5::Widgets
        ${PROJECT_LIBS}
)
