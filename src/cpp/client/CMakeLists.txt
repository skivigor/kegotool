find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Quick SerialPort Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Quick SerialPort Test)

#MESSAGE(STATUS "!!!!!!! In client: " ${TEST_ENABLED})

set(CLIENT_SOURCES
    qmlenvcreator.h qmlenvcreator.cpp
    serialclient.h serialclient.cpp
    bytetablemodel.h bytetablemodel.cpp
    commandmodel.h commandmodel.cpp
    commandstorage.h commandstorage.cpp
    iclient.h
)

if (TEST_ENABLED)
    set(CLIENT_SOURCES
        ${CLIENT_SOURCES}
        test_bytetablemodel.h test_bytetablemodel.cpp)
endif()

add_library(Client STATIC ${CLIENT_SOURCES})

target_link_libraries(Client
  PRIVATE
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Quick
  Qt${QT_VERSION_MAJOR}::SerialPort
  Qt${QT_VERSION_MAJOR}::Test
  Util
  Stor
)


