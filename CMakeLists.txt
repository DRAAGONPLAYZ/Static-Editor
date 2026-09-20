cmake_minimum_required(VERSION 3.21)

project(StaticEditor
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(PkgConfig REQUIRED)

pkg_check_modules(FFMPEG REQUIRED IMPORTED_TARGET
    libavcodec
    libavformat
    libavutil
    libswscale
    libswresample
)

add_executable(StaticEditorCore
    scripts/main.cpp
)

target_link_libraries(StaticEditorCore PRIVATE
    Qt6::Widgets
    PkgConfig::FFMPEG
)

target_compile_definitions(StaticEditorCore PRIVATE
    STATIC_EDITOR_VERSION="0.1.0"
)

if(MSVC)
    target_compile_options(StaticEditorCore PRIVATE /W4 /permissive-)
else()
    target_compile_options(StaticEditorCore PRIVATE -Wall -Wextra -Wpedantic)
endif()

if(WIN32)
    add_executable(StaticEditor
        scripts/launcher.cpp
    )

    target_compile_features(StaticEditor PRIVATE cxx_std_20)
    target_compile_options(StaticEditor PRIVATE /W4 /permissive-)
else()
    add_executable(StaticEditor ALIAS StaticEditorCore)
endif()

install(TARGETS StaticEditorCore RUNTIME DESTINATION bin)
