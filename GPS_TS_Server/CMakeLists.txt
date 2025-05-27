cmake_minimum_required(VERSION 3.16)

project(GPS_TS_Server VERSION 1.0.0 LANGUAGES CXX)

qt_add_executable(${PROJECT_NAME})

set(SERVER_SOURCES
    main.cpp
	Settings.cpp
	Application.cpp 
	TcpServer.cpp
	SerialPort.cpp
)

set(SERVER_HEADERS
	Settings.h
	Application.h
	TcpServer.h
	SerialPort.h
)

set(PARSER_SOURCES
	parser/NmeaGGA.cpp
	parser/NmeaGLL.cpp
	parser/NmeaGSA.cpp
	parser/NmeaGSV.cpp
	parser/NmeaPMTK.cpp
	parser/NmeaRMC.cpp
	parser/NmeaVTG.cpp
)

set(PARSER_HEADERS
	parser/NmeaGGA.h
	parser/NmeaGLL.h
	parser/NmeaGSA.h
	parser/NmeaGSV.h
	parser/NmeaPMTK.h
	parser/NmeaRMC.h
	parser/NmeaVTG.h
	parser/NmeaIParser.h
	parser/NmeaParserFactory.h
)

target_sources(${PROJECT_NAME} 
	PUBLIC 
		${SERVER_SOURCES} 
		${SERVER_HEADERS}
		${PARSER_SOURCES} 
		${PARSER_HEADERS}
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        Qt${QT_VERSION_MAJOR}::Core
		Qt${QT_VERSION_MAJOR}::Network
		Qt${QT_VERSION_MAJOR}::SerialPort
		Common
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" 
	PREFIX "Header Files"
    FILES
        ${SERVER_HEADERS}
		${CMAKE_CURRENT_SOURCE_DIR}/parser/${PARSER_HEADERS}
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}"
    PREFIX "Source Files"
    FILES
	    ${SERVER_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/parser/${PARSER_SOURCES}
)

target_include_directories(${PROJECT_NAME} 
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/parser
		${CMAKE_CURRENT_BINARY_DIR}
)

if (MSVC)
    target_compile_options(GPS_TS_Server PRIVATE /W4 /WX)
else ()
    target_compile_options(GPS_TS_Server PRIVATE -Wall -Wextra -pedantic)
endif()

