set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(PLUGIN "Pass")

set(
        SRC
        plugin.cpp
        pass.cpp
        gpg.cpp
        passkeymodel.h
        passphraseprovider.h
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

set(EXTERNAL_LIBS "${CMAKE_SOURCE_DIR}/build/${ARCH_TRIPLET}/gpg/local/")

INCLUDE_DIRECTORIES(${EXTERNAL_LIBS}/include)

add_library(GpgError STATIC IMPORTED)
set_property(TARGET GpgError PROPERTY IMPORTED_LOCATION "${EXTERNAL_LIBS}/lib/libgpg-error.a")

add_library(GpgAssuan STATIC IMPORTED)
set_property(TARGET GpgAssuan PROPERTY IMPORTED_LOCATION "${EXTERNAL_LIBS}/lib/libassuan.a")

add_library(Gpgme STATIC IMPORTED)
set_property(TARGET Gpgme PROPERTY IMPORTED_LOCATION "${EXTERNAL_LIBS}/lib/libgpgme.a")

add_library(Gpgmepp STATIC IMPORTED)
set_property(TARGET Gpgmepp PROPERTY IMPORTED_LOCATION "${EXTERNAL_LIBS}/lib/libgpgmepp.a")

add_library(QGpgme STATIC IMPORTED)
set_property(TARGET QGpgme PROPERTY IMPORTED_LOCATION "${EXTERNAL_LIBS}/lib/libqgpgme.a")

target_link_libraries(${PLUGIN} QGpgme Gpgmepp Gpgme GpgAssuan GpgError)


set(QT_IMPORTS_DIR "/lib/${ARCH_TRIPLET}")

install(TARGETS ${PLUGIN} DESTINATION ${QT_IMPORTS_DIR}/${PLUGIN}/)
install(FILES qmldir DESTINATION ${QT_IMPORTS_DIR}/${PLUGIN}/)
