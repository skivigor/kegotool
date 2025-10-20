find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Quick Concurrent Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Quick Concurrent Test)


#MESSAGE(STATUS "!!!!!!! In checksum: " ${TEST_ENABLED})

set(CHECKSUM_SOURCES
        qmlenvcreator.h qmlenvcreator.cpp
        crcengine.h crcengine.cpp
        crccalc.h crccalc.cpp
        hashcalc.h hashcalc.cpp
)

if (TEST_ENABLED)
    set(CHECKSUM_SOURCES
        ${CHECKSUM_SOURCES}
        test_crccalc.h test_crccalc.cpp
        test_hashcalc.h test_hashcalc.cpp)
endif()

add_library(Checksum STATIC ${CHECKSUM_SOURCES})

target_link_libraries(Checksum
  PRIVATE
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Quick
  Qt${QT_VERSION_MAJOR}::Concurrent
  Qt${QT_VERSION_MAJOR}::Test
  Util
)


