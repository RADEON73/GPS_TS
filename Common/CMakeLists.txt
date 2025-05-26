cmake_minimum_required(VERSION 3.16)

project(Common VERSION 1.0.0 LANGUAGES CXX)

set(COMMON_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/Logger.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TimeSynchronizer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Observer.cpp
)

set(COMMON_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/Logger.h
    ${CMAKE_CURRENT_SOURCE_DIR}/TimeSynchronizer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/encoding_fix.h
	${CMAKE_CURRENT_SOURCE_DIR}/Observer.h
)

add_library(Common STATIC ${COMMON_SOURCES} ${COMMON_HEADERS})

target_link_libraries(Common 
	PUBLIC
		Qt${QT_VERSION_MAJOR}::Core
		Qt${QT_VERSION_MAJOR}::Network
)

target_include_directories(Common
    PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}
		${CMAKE_CURRENT_BINARY_DIR}
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" 
	PREFIX "Header Files"
    FILES
        ${COMMON_HEADERS}
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}"
    PREFIX "Source Files"
    FILES
        ${COMMON_SOURCES}
)

set_target_properties(Common PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

if (MSVC)
    target_compile_options(Common PRIVATE /W4 /WX)
else ()
    target_compile_options(Common PRIVATE -Wall -Wextra -pedantic)
endif()
