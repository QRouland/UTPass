project(UTPass C CXX)
cmake_minimum_required(VERSION 3.5.1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

execute_process(
        COMMAND dpkg-architecture -qDEB_HOST_ARCH
        OUTPUT_VARIABLE CLICK_ARCH
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
        COMMAND dpkg-architecture -qDEB_HOST_MULTIARCH
        OUTPUT_VARIABLE ARCH_TRIPLET
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

find_package(Qt5Core)
find_package(Qt5Qml)
find_package(Qt5Quick)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Automatically create moc files
set(CMAKE_AUTOMOC ON)
set(QT_IMPORTS_DIR "lib/${ARCH_TRIPLET}")
set(PROJECT_NAME "UTPass")
set(FULL_PROJECT_NAME "utpass.qrouland")
set(CMAKE_INSTALL_PREFIX /)
set(DATA_DIR /)

set(DESKTOP_FILE_NAME ${PROJECT_NAME}.desktop)

if(NOT TESTS_PATH)
    set(TESTS_PATH "./tests")
endif()

configure_file(main.in.cpp main.cpp)

add_executable(${PROJECT_NAME} main.cpp)
qt5_use_modules(${PROJECT_NAME} Gui Qml Quick QuickTest)

if(TESTS_RUNNER)
    qt5_use_modules(${PROJECT_NAME} QuickTest)
endif()


install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})

configure_file(manifest.json.in ${CMAKE_CURRENT_BINARY_DIR}/manifest.json)
configure_file(${CMAKE_CURRENT_BINARY_DIR}/manifest.json ${CMAKE_CURRENT_BINARY_DIR}/manifest_.json COPYONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/manifest.json DESTINATION ${CMAKE_INSTALL_PREFIX})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/manifest_.json DESTINATION ${DATA_DIR})


install(FILES ${PROJECT_NAME}.apparmor DESTINATION ${DATA_DIR})
install(FILES ${PROJECT_NAME}.contenthub DESTINATION ${DATA_DIR})
install(FILES LICENSE DESTINATION ${DATA_DIR})
install(DIRECTORY qml DESTINATION ${DATA_DIR})
install(DIRECTORY assets DESTINATION ${DATA_DIR})


# Translations
file(GLOB_RECURSE I18N_SRC_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/po qml/*.qml qml/*.js)
list(APPEND I18N_SRC_FILES ${DESKTOP_FILE_NAME}.in.h)

find_program(INTLTOOL_MERGE intltool-merge)
if (NOT INTLTOOL_MERGE)
    message(FATAL_ERROR "Could not find intltool-merge, please install the intltool package")
endif ()
find_program(INTLTOOL_EXTRACT intltool-extract)
if (NOT INTLTOOL_EXTRACT)
    message(FATAL_ERROR "Could not find intltool-extract, please install the intltool package")
endif ()

add_custom_target(${DESKTOP_FILE_NAME} ALL
        COMMENT "Merging translations into ${DESKTOP_FILE_NAME}..."
        COMMAND LC_ALL=C ${INTLTOOL_MERGE} -d -u ${CMAKE_SOURCE_DIR}/po ${CMAKE_SOURCE_DIR}/${DESKTOP_FILE_NAME}.in ${DESKTOP_FILE_NAME}
        COMMAND sed -i 's/${PROJECT_NAME}-//g' ${CMAKE_CURRENT_BINARY_DIR}/${DESKTOP_FILE_NAME}
        )

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${DESKTOP_FILE_NAME} DESTINATION ${DATA_DIR})

add_subdirectory(po)
add_subdirectory(plugins)
if(TESTS_RUNNER)
    add_subdirectory(tests/plugins)
endif()


add_custom_target(${PROJECT_NAME}_FILES ALL SOURCES ${PROJECT_SRC_FILES})
