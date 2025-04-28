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
		SerialPort
)

qt_standard_project_setup()

qt_add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PUBLIC
    main.cpp
    core/ConfigManager.cpp core/ConfigManager.h
	core/Application.cpp core/Application.h
    network/TcpServer.cpp network/TcpServer.h
    network/SerialPort.cpp network/SerialPort.h
	parser/NmeaGGA.cpp parser/NmeaGGA.h
	parser/NmeaGLL.cpp parser/NmeaGLL.h
	parser/NmeaGSA.cpp parser/NmeaGSA.h
	parser/NmeaGSV.cpp parser/NmeaGSV.h
	parser/NmeaPMTK.cpp parser/NmeaPMTK.h
	parser/NmeaRMC.cpp parser/NmeaRMC.h
	parser/NmeaVTG.cpp parser/NmeaVTG.h
	parser/NmeaIParser.h
	parser/NmeaParserFactory.h
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        Qt::Core
		Qt::Network
		Qt::SerialPort
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" 
    PREFIX "Header Files" 
    FILES 
        ${CMAKE_CURRENT_SOURCE_DIR}/core/ConfigManager.h
		${CMAKE_CURRENT_SOURCE_DIR}/core/Application.h
        ${CMAKE_CURRENT_SOURCE_DIR}/network/TcpServer.h
        ${CMAKE_CURRENT_SOURCE_DIR}/network/SerialPort.h
		${CMAKE_CURRENT_SOURCE_DIR}/parser/NmeaGGA.h
		${CMAKE_CURRENT_SOURCE_DIR}/parser/NmeaGLL.h
		${CMAKE_CURRENT_SOURCE_DIR}/parser/NmeaGSA.h
		${CMAKE_CURRENT_SOURCE_DIR}/parser/NmeaGSV.h
		${CMAKE_CURRENT_SOURCE_DIR}/parser/NmeaPMTK.h
		${CMAKE_CURRENT_SOURCE_DIR}/parser/NmeaRMC.h
		${CMAKE_CURRENT_SOURCE_DIR}/parser/NmeaVTG.h
		${CMAKE_CURRENT_SOURCE_DIR}/parser/NmeaIParser.h
		${CMAKE_CURRENT_SOURCE_DIR}/parser/NmeaParserFactory.h
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" 
    PREFIX "Source Files" 
    FILES 
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/core/ConfigManager.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/core/Application.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/network/TcpServer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/network/SerialPort.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/parser/NmeaGGA.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/parser/NmeaGLL.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/parser/NmeaGSA.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/parser/NmeaGSV.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/parser/NmeaPMTK.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/parser/NmeaRMC.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/parser/NmeaVTG.cpp
)

# Установка стандарта C++
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

