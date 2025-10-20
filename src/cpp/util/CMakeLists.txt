find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui)

set(UTIL_SOURCES
        fsutils.h fsutils.cpp
        byteutils.h byteutils.cpp
#        testclass.h testclass.cpp
        unsortedmap.h
)

if (TEST_ENABLED)
    MESSAGE(STATUS "!!!!!!! Utils Enabled: ")
    set(UTIL_SOURCES
        ${UTIL_SOURCES}
        testclass.h testclass.cpp)
endif()

add_library(Util STATIC ${UTIL_SOURCES})

target_link_libraries(Util
  PRIVATE
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Gui
)


