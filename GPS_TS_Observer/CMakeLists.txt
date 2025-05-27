cmake_minimum_required(VERSION 3.16)

project(GPS_TS_Observer VERSION 1.0.0 LANGUAGES CXX)

qt_add_executable(${PROJECT_NAME})

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

# Ресурсы (Qt5-совместимый синтаксис)
qt5_add_resources(RESOURCES_RCC
    MainWindow.qrc
)

target_sources(${PROJECT_NAME} PRIVATE
    ${SOURCES}
    ${HEADERS}
    ${FORMS}
    ${RESOURCES_RCC}
)

# Линкуем необходимые библиотеки Qt
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Gui
		Qt${QT_VERSION_MAJOR}::Network
)

if(WIN32)
    target_sources(${PROJECT_NAME} PRIVATE
        GPS_TS_Observer.rc
    )
	
    set_target_properties(${PROJECT_NAME} PROPERTIES
        SUFFIX ".exe"
		WIN32_EXECUTABLE TRUE
    )
    
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/GPS_TS_Observer.ico")
        set_target_properties(${PROJECT_NAME} PROPERTIES
            RC_ICONS "${CMAKE_CURRENT_SOURCE_DIR}/GPS_TS_Observer.ico"
        )
    endif()
endif()

if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
endif()

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
)

# Группировка файлов в Visual Studio
source_group("Header Files" FILES ${HEADERS})
source_group("Source Files" FILES ${SOURCES})
source_group("Form Files" FILES ${FORMS})
source_group("Resource Files" FILES ${RESOURCES})