cmake_minimum_required(VERSION 3.16)

project("GPS_TS" VERSION 1.0.0 LANGUAGES CXX)

# Общие настройки для всего проекта
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(qt.cmake)

find_package(QT NAMES Qt5 Qt6 REQUIRED COMPONENTS Core Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Network)

qt_standard_project_setup()

if(MSVC)
    # Для статической линковки CRT
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
endif()

if(WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /MANIFEST:NO")
    add_definitions(-DUNICODE -D_UNICODE)
    set(QT_LINK_STATIC ON)
    add_compile_definitions("_WIN32_WINNT=0x0A00" "WINVER=0x0A00")
endif()

add_subdirectory("GPS_TS_Server")
add_subdirectory("GPS_TS_Client")