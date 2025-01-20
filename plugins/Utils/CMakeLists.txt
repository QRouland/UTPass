set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(PLUGIN "Utils")

set(
        SRC
        plugin.cpp
        utils.cpp
        jobs/unzipjob.cpp
)

set(CMAKE_AUTOMOC ON)

execute_process(
        COMMAND dpkg-architecture -qDEB_HOST_MULTIARCH
        OUTPUT_VARIABLE ARCH_TRIPLET
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

if(ARCH_TRIPLET STREQUAL "")
    set(ARCH_TRIPLET x86_64-linux-gnu)
endif()

add_library(${PLUGIN} MODULE ${SRC})
set_target_properties(${PLUGIN} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PLUGIN})
qt5_use_modules(${PLUGIN} Qml Quick DBus)

add_library(libquazip5 SHARED IMPORTED)
set_property(TARGET libquazip5 PROPERTY IMPORTED_LOCATION "/usr/lib/${ARCH_TRIPLET}/libquazip5.so")

target_link_libraries(${PLUGIN} libquazip5)

set(QT_IMPORTS_DIR "/lib/${ARCH_TRIPLET}")

install(TARGETS ${PLUGIN} DESTINATION ${QT_IMPORTS_DIR}/${PLUGIN}/)
install(FILES qmldir DESTINATION ${QT_IMPORTS_DIR}/${PLUGIN}/)
