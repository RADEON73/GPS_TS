cmake_minimum_required(VERSION 3.16)

project(GPS_TS_Client VERSION 1.0.0 LANGUAGES CXX)

qt_add_executable(${PROJECT_NAME})

set(CLIENT_SOURCES
    main.cpp
    Settings.cpp
	Application.cpp 
    TcpClient.cpp 
)

set(CLIENT_HEADERS
	Settings.h
	Application.h
	TcpClient.h
)

target_sources(${PROJECT_NAME} 
	PUBLIC 
		${CLIENT_SOURCES} 
		${CLIENT_HEADERS}
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        Qt${QT_VERSION_MAJOR}::Core
		Qt${QT_VERSION_MAJOR}::Network
		Common
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" 
	PREFIX "Header Files"
    FILES
        ${CLIENT_HEADERS}
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}"
    PREFIX "Source Files"
    FILES
        ${CLIENT_SOURCES}
)

# Установка стандарта C++
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(${PROJECT_NAME} 
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
		${CMAKE_CURRENT_BINARY_DIR}
)

if (MSVC)
    target_compile_options(GPS_TS_Client PRIVATE /W4)
else ()
    target_compile_options(GPS_TS_Client PRIVATE -Wall -Wextra -pedantic)
endif()

