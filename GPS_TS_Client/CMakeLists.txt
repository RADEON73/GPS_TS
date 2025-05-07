cmake_minimum_required(VERSION 3.16)
project(GPS_TS_Client VERSION 1.0.0 LANGUAGES CXX)

qt_add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PUBLIC
    main.cpp
    core/Settings.cpp core/Settings.h
	core/Application.cpp core/Application.h
	core/TimeSynchronizer.cpp core/TimeSynchronizer.h
	core/Logger.cpp core/Logger.h
    network/TcpClient.cpp network/TcpClient.h
	encoding_fix.h
)

target_link_libraries(${PROJECT_NAME}
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
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

