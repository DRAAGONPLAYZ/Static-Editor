cmake_minimum_required(VERSION 3.21)

project(StaticEditor
    VERSION 1.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

if(WIN32)
    set(FFMPEG_ROOT "$ENV{MINGW_PREFIX}")

    find_path(FFMPEG_INCLUDE_DIR
        NAMES libavcodec/avcodec.h
        PATHS "${FFMPEG_ROOT}/include"
        REQUIRED
    )

    find_library(AVCODEC_LIBRARY NAMES avcodec PATHS "${FFMPEG_ROOT}/lib" REQUIRED)
    find_library(AVFORMAT_LIBRARY NAMES avformat PATHS "${FFMPEG_ROOT}/lib" REQUIRED)
    find_library(AVUTIL_LIBRARY NAMES avutil PATHS "${FFMPEG_ROOT}/lib" REQUIRED)
    find_library(SWSCALE_LIBRARY NAMES swscale PATHS "${FFMPEG_ROOT}/lib" REQUIRED)
    find_library(SWRESAMPLE_LIBRARY NAMES swresample PATHS "${FFMPEG_ROOT}/lib" REQUIRED)

    add_library(FFMPEG::FFMPEG INTERFACE IMPORTED)
    target_include_directories(FFMPEG::FFMPEG INTERFACE "${FFMPEG_INCLUDE_DIR}")
    target_link_libraries(FFMPEG::FFMPEG INTERFACE
        "${AVCODEC_LIBRARY}"
        "${AVFORMAT_LIBRARY}"
        "${AVUTIL_LIBRARY}"
        "${SWSCALE_LIBRARY}"
        "${SWRESAMPLE_LIBRARY}"
    )
else()
    find_package(PkgConfig REQUIRED)

    pkg_check_modules(FFMPEG REQUIRED IMPORTED_TARGET
        libavcodec
        libavformat
        libavutil
        libswscale
        libswresample
    )
endif()

add_executable(StaticEditorCore
    scripts/main.cpp
    scripts/logger.cpp
    scripts/logger.h
    scripts/user_directories.cpp
    scripts/user_directories.h
    scripts/opening_screen.cpp
    scripts/opening_screen.h
    scripts/editor_screen.cpp
    scripts/editor_screen.h
    scripts/settings_manager.cpp
    scripts/settings_manager.h
    scripts/settings_window.cpp
    scripts/settings_window.h
    assets/resources.qrc
)

target_link_libraries(StaticEditorCore PRIVATE
    Qt6::Widgets
    FFMPEG::FFMPEG
)

if(WIN32)
    target_link_libraries(StaticEditorCore PRIVATE advapi32)
endif()

target_compile_definitions(StaticEditorCore PRIVATE
    STATIC_EDITOR_VERSION="1.1.0"
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

    if(MSVC)
        target_compile_options(StaticEditor PRIVATE /W4 /permissive-)
    else()
        target_compile_options(StaticEditor PRIVATE -Wall -Wextra -Wpedantic)
    endif()
else()
    add_executable(StaticEditor ALIAS StaticEditorCore)
endif()

install(TARGETS StaticEditorCore RUNTIME DESTINATION bin)
