set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(PLUGIN "Pass")

set(
        SRC
        plugin.cpp
        pass.cpp
        passkeymodel.h
        jobs/rmjob.cpp
        jobs/rnpjob.cpp
        jobs/getkeysjob.cpp
        jobs/importkeyjob.cpp
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



set(RNP_BUILD_DIR "${CMAKE_SOURCE_DIR}/build/${ARCH_TRIPLET}/rnp/install")

find_package(OpenSSL REQUIRED)
find_package(JSON-C 0.11)

INCLUDE_DIRECTORIES(${RNP_BUILD_DIR}/include)

add_library(rnp STATIC IMPORTED)
set_property(TARGET rnp PROPERTY IMPORTED_LOCATION "${RNP_BUILD_DIR}/lib/librnp.a")

add_library(sexpp STATIC IMPORTED)
set_property(TARGET sexpp PROPERTY IMPORTED_LOCATION "${RNP_BUILD_DIR}/lib/libsexpp.a")

add_library(gpgerror SHARED IMPORTED)
set_property(TARGET gpgerror PROPERTY IMPORTED_LOCATION "/usr/lib/${ARCH_TRIPLET}/libgpg-error.so.0.28.0")

add_library(libassuan SHARED IMPORTED)
set_property(TARGET libassuan PROPERTY IMPORTED_LOCATION "/usr/lib/${ARCH_TRIPLET}/libassuan.so")

add_library(libgpgme SHARED IMPORTED)
set_property(TARGET libgpgme PROPERTY IMPORTED_LOCATION "/usr/lib/${ARCH_TRIPLET}/libgpgme.so")

add_library(libgpgmepp SHARED IMPORTED)
set_property(TARGET libgpgmepp PROPERTY IMPORTED_LOCATION "/usr/lib/${ARCH_TRIPLET}/libgpgmepp.so")

add_library(libqgpgme SHARED IMPORTED)
set_property(TARGET libqgpgme PROPERTY IMPORTED_LOCATION "/usr/lib/${ARCH_TRIPLET}/libqgpgme.so")


target_link_libraries(${PLUGIN} rnp sexpp gpgerror libassuan libgpgme libgpgmepp libqgpgme OpenSSL::Crypto JSON-C::JSON-C)

set(QT_IMPORTS_DIR "/lib/${ARCH_TRIPLET}")
install(TARGETS ${PLUGIN} DESTINATION ${QT_IMPORTS_DIR}/${PLUGIN}/)
install(FILES qmldir DESTINATION ${QT_IMPORTS_DIR}/${PLUGIN}/)
