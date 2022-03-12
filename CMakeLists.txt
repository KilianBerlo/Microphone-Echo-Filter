cmake_minimum_required(VERSION 3.0.0)
project(Microphone_Echo_Filter VERSION 0.1.0)

find_package(PkgConfig REQUIRED)
find_package(Threads)
pkg_check_modules(GST REQUIRED IMPORTED_TARGET  gstreamer-1.0)
pkg_check_modules(GSTAUDIO REQUIRED IMPORTED_TARGET  gstreamer-audio-1.0)
pkg_check_modules(GSTAPP REQUIRED IMPORTED_TARGET  gstreamer-app-1.0)

add_executable(Microphone_Echo_Filter main.cpp lib/gstreamer.cpp)

target_link_libraries(Microphone_Echo_Filter PUBLIC
        PkgConfig::GST
        PkgConfig::GSTAUDIO
        PkgConfig::GSTAPP
        ${CMAKE_THREAD_LIBS_INIT}
        )

#target_link_libraries (Microphone_Echo_Filter ${CMAKE_THREAD_LIBS_INIT})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
