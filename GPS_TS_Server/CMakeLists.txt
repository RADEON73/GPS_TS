cmake_minimum_required(VERSION 3.16)
project(GPS_TS_Server VERSION 1.0.0 LANGUAGES CXX)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS SerialPort)

qt_add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PUBLIC
    main.cpp
    core/Settings.cpp core/Settings.h
	core/Application.cpp core/Application.h
	core/TimeSynchronizer.cpp core/TimeSynchronizer.h
	core/Logger.cpp core/Logger.h
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
	encoding_fix.h
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        Qt${QT_VERSION_MAJOR}::Core
		Qt${QT_VERSION_MAJOR}::Network
		Qt${QT_VERSION_MAJOR}::SerialPort
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" 
    PREFIX "Header Files" 
    FILES 
        ${CMAKE_CURRENT_SOURCE_DIR}/core/Settings.h
		${CMAKE_CURRENT_SOURCE_DIR}/core/Application.h
		${CMAKE_CURRENT_SOURCE_DIR}/core/TimeSynchronizer.h
		${CMAKE_CURRENT_SOURCE_DIR}/core/Logger.h
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
        ${CMAKE_CURRENT_SOURCE_DIR}/core/Settings.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/core/Application.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/core/TimeSynchronizer.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/core/Logger.cpp
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

