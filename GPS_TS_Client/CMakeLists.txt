cmake_minimum_required(VERSION 3.16)
project(GPS_TS LANGUAGES CXX)

include(qt.cmake)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Network SerialPort)
find_package(Qt${QT_VERSION_MAJOR}
    COMPONENTS
        Core
		Network
)

qt_standard_project_setup()

if(MSVC)
    # Для статической линковки CRT
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
	# Для статической CRT
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
endif()

if(WIN32)
    # Добавляем манифест для поддержки UTF-8 в Windows
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /MANIFEST:NO")
    add_definitions(-DUNICODE -D_UNICODE)
	
	set(QT_LINK_STATIC ON)
    
    # Для новых версий Windows (10+)
    add_compile_definitions("_WIN32_WINNT=0x0A00")
    add_compile_definitions("WINVER=0x0A00")
endif()

qt_add_executable(${PROJECT_NAME}_Client)

target_sources(${PROJECT_NAME}_Client PUBLIC
    main.cpp
    core/Settings.cpp core/Settings.h
	core/Application.cpp core/Application.h
	core/TimeSynchronizer.cpp core/TimeSynchronizer.h
	core/Logger.cpp core/Logger.h
    network/TcpClient.cpp network/TcpClient.h
	encoding_fix.h
)

target_link_libraries(${PROJECT_NAME}_Client
    PUBLIC
        Qt${QT_VERSION_MAJOR}::Core
		Qt${QT_VERSION_MAJOR}::Network
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" 
    PREFIX "Header Files" 
    FILES 
        ${CMAKE_CURRENT_SOURCE_DIR}/core/Settings.h
		${CMAKE_CURRENT_SOURCE_DIR}/core/Application.h
		${CMAKE_CURRENT_SOURCE_DIR}/core/TimeSynchronizer.h
		${CMAKE_CURRENT_SOURCE_DIR}/core/Logger.h
        ${CMAKE_CURRENT_SOURCE_DIR}/network/TcpClient.h
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" 
    PREFIX "Source Files" 
    FILES 
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/core/Settings.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/core/Application.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/core/TimeSynchronizer.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/core/Logger.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/network/TcpClient.cpp
)

# Установка стандарта C++
set_target_properties(${PROJECT_NAME}_Client PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

