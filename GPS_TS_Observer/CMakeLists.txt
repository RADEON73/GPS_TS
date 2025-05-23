cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
	
project(GPS_TS_Observer VERSION 1.0.0 LANGUAGES CXX)
	
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets Gui)
		
if(QT_VERSION_MAJOR EQUAL 6)
    qt_standard_project_setup()
else()
    qt_standard_project_setup()
	set(CMAKE_AUTOMOC ON)
	set(CMAKE_AUTOUIC ON)
	set(CMAKE_AUTORCC ON) 
endif()

qt_add_executable(${PROJECT_NAME})

# Ресурсы Qt
set(RESOURCES
	MainWindow.qrc
)

# UI формы
set(FORMS
    MainWindow.ui
)

# Заголовочные файлы
set(HEADERS
    MainWindow.h
)

# Исходные файлы
set(SOURCES
    main.cpp
    MainWindow.cpp
)

# Добавляем исходные файлы
target_sources(${PROJECT_NAME}
    PRIVATE
        ${SOURCES}
        ${HEADERS}
        ${FORMS}
		${RESOURCES}
)

# Добавляем ресурсы в проект
qt_add_resources(SOURCES ${RESOURCES})

# Линкуем необходимые библиотеки Qt
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Gui
)

# Группировка файлов в Visual Studio
source_group("Header Files" FILES ${HEADERS})
source_group("Source Files" FILES ${SOURCES})
source_group("Form Files" FILES ${FORMS})
source_group("Resource Files" FILES ${RESOURCES})

# Установка стандарта C++
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    WIN32_EXECUTABLE TRUE
)

# Настройки для Windows
if(WIN32)
    target_sources(${PROJECT_NAME} PRIVATE
        GPS_TS_Observer.rc
    )
	
    set_target_properties(${PROJECT_NAME} PROPERTIES
        SUFFIX ".exe"
    )
    
    # Иконка приложения
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/GPS_TS_Observer.ico")
        set_target_properties(${PROJECT_NAME} PROPERTIES
            RC_ICONS "${CMAKE_CURRENT_SOURCE_DIR}/GPS_TS_Observer.ico"
        )
    endif()
endif()

# Настройки компилятора
if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
endif()

# Директории включения
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
)