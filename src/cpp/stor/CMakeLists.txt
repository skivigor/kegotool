find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Qml Gui Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Qml Gui Sql)

set(STOR_SOURCES
        fileloader.h fileloader.cpp
        jdatabase.h jdatabase.cpp
        configjsondbstorage.h configjsondbstorage.cpp
        appstyle.h appstyle.cpp
)

add_library(Stor STATIC ${STOR_SOURCES})

target_link_libraries(Stor
  PRIVATE
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Qml
  Qt${QT_VERSION_MAJOR}::Gui
  Qt${QT_VERSION_MAJOR}::Sql)


