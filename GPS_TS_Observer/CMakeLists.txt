cmake_minimum_required(VERSION 3.16)

project(GPS_TS_Observer VERSION 1.0.0 LANGUAGES CXX)

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
		Qt${QT_VERSION_MAJOR}::Network
)

# Группировка файлов в Visual Studio
source_group("Header Files" FILES ${HEADERS})
source_group("Source Files" FILES ${SOURCES})
source_group("Form Files" FILES ${FORMS})
source_group("Resource Files" FILES ${RESOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    WIN32_EXECUTABLE TRUE
)

if(WIN32)
    target_sources(${PROJECT_NAME} PRIVATE
        GPS_TS_Observer.rc
    )
	
    set_target_properties(${PROJECT_NAME} PROPERTIES
        SUFFIX ".exe"
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