set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(PLUGIN "Pass")

set(
    SRC
    plugin.cpp
    pass.cpp
)

set(CMAKE_AUTOMOC ON)

add_library(${PLUGIN} MODULE ${SRC})
set_target_properties(${PLUGIN} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PLUGIN})
qt5_use_modules(${PLUGIN} Qml Quick DBus)


set(EXTERNAL_LIBS "${CMAKE_SOURCE_DIR}/local")
set(THIRD_PATH "${CMAKE_CURRENT_SOURCE_DIR}")

INCLUDE_DIRECTORIES(${EXTERNAL_LIBS}/include)
add_dependencies(${PLUGIN} LibGpgme)

add_library(target-LibGpgError STATIC IMPORTED)
set_property(TARGET target-LibGpgError PROPERTY IMPORTED_LOCATION "${EXTERNAL_LIBS}/lib/libgpg-error.a")

add_library(target-LibGpgAssuan STATIC IMPORTED)
set_property(TARGET target-LibGpgAssuan PROPERTY IMPORTED_LOCATION "${EXTERNAL_LIBS}/lib/libassuan.a")

add_library(target-LibGpgppme STATIC IMPORTED)
set_property(TARGET target-LibGpgppme PROPERTY IMPORTED_LOCATION "${EXTERNAL_LIBS}/lib/libgpgmepp.a")

add_library(target-LibQGpgme STATIC IMPORTED)
set_property(TARGET target-LibQGpgme PROPERTY IMPORTED_LOCATION "${EXTERNAL_LIBS}/lib/libqgpgme.a")

target_link_libraries(${PLUGIN} target-LibGpgAssuan target-LibGpgError target-LibGpgppme target-LibQGpgme)

execute_process(
    COMMAND dpkg-architecture -qDEB_HOST_MULTIARCH
    OUTPUT_VARIABLE ARCH_TRIPLET
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

set(QT_IMPORTS_DIR "/lib/${ARCH_TRIPLET}")

install(TARGETS ${PLUGIN} DESTINATION ${QT_IMPORTS_DIR}/${PLUGIN}/)
install(FILES qmldir DESTINATION ${QT_IMPORTS_DIR}/${PLUGIN}/)
