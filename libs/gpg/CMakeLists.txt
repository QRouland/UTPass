cmake_minimum_required(VERSION 3.5.1)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)

execute_process(
        COMMAND dpkg-architecture -qDEB_HOST_MULTIARCH
        OUTPUT_VARIABLE ARCH_TRIPLET
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process (
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/clean.sh ${ARCH_TRIPLET}
)

set(EXTERNAL_LIBS "${CMAKE_CURRENT_BINARY_DIR}/local/")
set(THIRD_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
set(LIBGPGERROR_PATH "${THIRD_PATH}/libgpg-error")
set(LIBASSUAN_PATH "${THIRD_PATH}/libassuan")
set(LIBGPGME_PATH "${THIRD_PATH}/gpgme")
set(GNUPG_PATH "${THIRD_PATH}/gnupg")

ExternalProject_Add(
        LibGpgError
        INSTALL_DIR ${EXTERNAL_LIBS}
        DOWNLOAD_COMMAND ""
        SOURCE_DIR ${LIBGPGERROR_PATH}
        CONFIGURE_COMMAND <SOURCE_DIR>/autogen.sh && <SOURCE_DIR>/configure --prefix=${EXTERNAL_LIBS} --enable-static=yes --enable-shared=no --with-pic=yes --enable-maintainer-mode --host ${ARCH_TRIPLET} --disable-doc --disable-dependency-tracking
        BUILD_IN_SOURCE 1
        BUILD_COMMAND make
        INSTALL_COMMAND make install
)

ExternalProject_Add(
        LibGpgAssuan
        DEPENDS LibGpgError
        INSTALL_DIR ${EXTERNAL_LIBS}
        DOWNLOAD_COMMAND ""
        SOURCE_DIR ${LIBASSUAN_PATH}
        CONFIGURE_COMMAND <SOURCE_DIR>/autogen.sh && <SOURCE_DIR>/configure --prefix=${EXTERNAL_LIBS} --enable-static=yes --enable-shared=no --with-pic=yes --enable-maintainer-mode --with-libgpg-error-prefix=${EXTERNAL_LIBS} --host ${ARCH_TRIPLET} --disable-doc --disable-dependency-tracking
        BUILD_IN_SOURCE 1
        BUILD_COMMAND make
        INSTALL_COMMAND make install
)

ExternalProject_Add(
        LibGpgme
        DEPENDS LibGpgError LibGpgAssuan
        INSTALL_DIR ${EXTERNAL_LIBS}
        DOWNLOAD_COMMAND ""
        SOURCE_DIR ${LIBGPGME_PATH}
        CONFIGURE_COMMAND <SOURCE_DIR>/autogen.sh && <SOURCE_DIR>/configure  --enable-languages=cpp,qt  --enable-static=yes --enable-shared=no --with-pic=yes --prefix=${EXTERNAL_LIBS} --enable-maintainer-mode --with-libgpg-error-prefix=${EXTERNAL_LIBS} --with-libassuan-prefix=${EXTERNAL_LIBS} --host ${ARCH_TRIPLET} --disable-doc --disable-largefile --disable-dependency-tracking
        BUILD_IN_SOURCE 1
        BUILD_COMMAND make
        INSTALL_COMMAND make install
)


ExternalProject_Add(
        Gnupg
        DEPENDS LibGpgError LibGpgAssuan
        INSTALL_DIR ${EXTERNAL_LIBS}
        DOWNLOAD_COMMAND ""
        SOURCE_DIR ${GNUPG_PATH}
        CONFIGURE_COMMAND <SOURCE_DIR>/autogen.sh && <SOURCE_DIR>/configure  --prefix=${EXTERNAL_LIBS} --with-libgpg-error-prefix=${EXTERNAL_LIBS} --with-libassuan-prefix=${EXTERNAL_LIBS} --enable-maintainer-mode --with-libassuan-prefix=${EXTERNAL_LIBS} --host ${ARCH_TRIPLET} --disable-doc --disable-dependency-tracking
        BUILD_IN_SOURCE 1 
        BUILD_COMMAND make
        INSTALL_COMMAND make install
)

install(TARGETS)
