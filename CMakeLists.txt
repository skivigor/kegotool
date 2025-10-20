cmake_minimum_required(VERSION 3.14)

project(KegoTool VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(TEST_ENABLED OFF)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Quick)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Quick)

#include_directories(../src)
include_directories(src/cpp)
add_subdirectory(src/cpp/util ${CMAKE_CURRENT_BINARY_DIR}/lib/util)
add_subdirectory(src/cpp/checksum ${CMAKE_CURRENT_BINARY_DIR}/lib/checksum)
add_subdirectory(src/cpp/stor ${CMAKE_CURRENT_BINARY_DIR}/lib/stor)
add_subdirectory(src/cpp/client ${CMAKE_CURRENT_BINARY_DIR}/lib/client)
add_subdirectory(src/cpp/script ${CMAKE_CURRENT_BINARY_DIR}/lib/script)


set(PROJECT_SOURCES
        projectconf.h
        main.cpp
        qml.qrc
        app.rc
)

#if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
#    qt_add_executable(KegoTool
#        MANUAL_FINALIZATION
#        ${PROJECT_SOURCES}
#    )
## Define target properties for Android with Qt 6 as:
##    set_property(TARGET KegoTool APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
##                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
## For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
#else()
#    if(ANDROID)
#        add_library(KegoTool SHARED
#            ${PROJECT_SOURCES}
#        )
## Define properties for Android with Qt 5 after find_package() calls as:
##    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    else()
#        add_executable(KegoTool
#          ${PROJECT_SOURCES}
#        )
#    endif()
#endif()

add_executable(KegoTool
  ${PROJECT_SOURCES}
)

target_link_libraries(KegoTool
  PRIVATE
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Quick
#  Qt${QT_VERSION_MAJOR}::Concurrent
#  Qt${QT_VERSION_MAJOR}::SerialPort
#  Qt${QT_VERSION_MAJOR}::Sql
  Util
  Checksum
  Stor
  Client
  Script
)

set_target_properties(KegoTool PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS KegoTool
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_import_qml_plugins(KegoTool)
    qt_finalize_executable(KegoTool)
endif()
