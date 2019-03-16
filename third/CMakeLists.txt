set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

execute_process(
        COMMAND dpkg-architecture -qDEB_HOST_MULTIARCH
        OUTPUT_VARIABLE ARCH_TRIPLET
        OUTPUT_STRIP_TRAILING_WHITESPACE
)


set(EXTERNAL_LIBS "${CMAKE_SOURCE_DIR}/local")
set(THIRD_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
set(ENV{PATH} "${EXTERNAL_LIBS}/bin")
set(LIBGPGERROR_PATH "${THIRD_PATH}/libgpg-error")
set(LIBASSUANSRC_PATH "${THIRD_PATH}/libassuan")
set(LIBGPGME_PATH "${THIRD_PATH}/gpgme")

#INCLUDE_DIRECTORIES(${EXTERNAL_LIBS}/include)
#LINK_DIRECTORIES(${EXTERNAL_LIBS}/lib)

ExternalProject_Add(
        LibGpgError
        INSTALL_DIR ${EXTERNAL_LIBS}
        DOWNLOAD_COMMAND ""
        SOURCE_DIR ${LIBGPGERROR_PATH}
        CONFIGURE_COMMAND <SOURCE_DIR>/autogen.sh && <SOURCE_DIR>/configure --prefix=${EXTERNAL_LIBS} --enable-static=yes --enable-shared=no --with-pic=yes --enable-maintainer-mode --host ${ARCH_TRIPLET}
        BUILD_IN_SOURCE 1
        BUILD_COMMAND make
        INSTALL_COMMAND make install
)

ExternalProject_Add(
        LibGpgAssuan
        DEPENDS LibGpgError
        INSTALL_DIR ${EXTERNAL_LIBS}
        DOWNLOAD_COMMAND ""
        SOURCE_DIR ${LIBASSUANSRC_PATH}
        CONFIGURE_COMMAND <SOURCE_DIR>/autogen.sh && <SOURCE_DIR>/configure --prefix=${EXTERNAL_LIBS} --enable-static=yes --enable-shared=no --with-pic=yes --enable-maintainer-mode --with-libgpg-error-prefix=${EXTERNAL_LIBS} --host ${ARCH_TRIPLET}
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
        CONFIGURE_COMMAND <SOURCE_DIR>/autogen.sh && <SOURCE_DIR>/configure  --enable-languages=cpp,qt  --enable-static=yes --enable-shared=no --with-pic=yes --prefix=${EXTERNAL_LIBS} --enable-maintainer-mode --with-libgpg-error-prefix=${EXTERNAL_LIBS} --with-libassuan-prefix=${EXTERNAL_LIBS} --host ${ARCH_TRIPLET}
        BUILD_IN_SOURCE 1
        BUILD_COMMAND make
        INSTALL_COMMAND make install
)