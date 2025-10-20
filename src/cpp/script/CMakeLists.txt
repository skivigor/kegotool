find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Quick)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Quick)

set(SCRIPT_SOURCES
        scriptengine.h scriptengine.cpp
        scriptexecutor.h scriptexecutor.cpp
        scriptlogmodel.h scriptlogmodel.cpp
        scriptfilemodel.h scriptfilemodel.cpp
        qmlenvcreator.h qmlenvcreator.cpp
)

add_library(Script STATIC ${SCRIPT_SOURCES})

target_link_libraries(Script
  PRIVATE
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Quick
)


