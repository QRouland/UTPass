set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(PLUGIN "Git")

set(
        SRC
        plugin.cpp
        git.cpp
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

set(EXTERNAL_LIBS "${CMAKE_SOURCE_DIR}/build/${ARCH_TRIPLET}/libgit2/install/")

INCLUDE_DIRECTORIES(${EXTERNAL_LIBS}/include)

add_library(libgit2 STATIC IMPORTED)
set_property(TARGET libgit2 PROPERTY IMPORTED_LOCATION "${EXTERNAL_LIBS}/lib/libgit2.a")

target_link_libraries(${PLUGIN} libgit2)


set(QT_IMPORTS_DIR "/lib/${ARCH_TRIPLET}")

install(TARGETS ${PLUGIN} DESTINATION ${QT_IMPORTS_DIR}/${PLUGIN}/)
install(FILES qmldir DESTINATION ${QT_IMPORTS_DIR}/${PLUGIN}/)
