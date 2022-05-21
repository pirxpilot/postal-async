cmake_minimum_required(VERSION 2.8...3.13)

set(BUILD_SHARED_LIBS ON CACHE BOOL "Build shared libraries")

# Name of the project (will be the name of the plugin)
project(postal-async)

add_definitions(-DNAPI_VERSION=${NAPI_VERSION})

add_library(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
  PRIVATE
    "${PROJECT_SOURCE_DIR}/src/postal.cc"
    "${PROJECT_SOURCE_DIR}/src/expand.h"
    "${PROJECT_SOURCE_DIR}/src/expand.cc"
    "${PROJECT_SOURCE_DIR}/src/parse.h"
    "${PROJECT_SOURCE_DIR}/src/parse.cc"
)

# Gives our library file a .node extension without any "lib" prefix
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")

# Essential include files to build a node addon,
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_JS_INC})

# Essential library files to link to a node addon
target_link_libraries(
  ${PROJECT_NAME}
  ${CMAKE_JS_LIB}
)

# build libtool

set(POSTAL_DATA_DIR $ENV{POSTAL_DATA_DIR})
set(POSTAL_DOWNLOAD_DATA $ENV{POSTAL_DOWNLOAD_DATA})

if(NOT POSTAL_DATA_DIR)
  set(POSTAL_DATA_DIR /var/lib)
endif()

# SSE / SSE2 isn't available on all platforms; arm, arm64 and arm/M1 (aarch64) are such examples
if(CMAKE_SYSTEM_PROCESSOR MATCHES "^arm" OR CMAKE_SYSTEM_PROCESSOR MATCHES "^aarch64")
  set(IS_SSE_DISABLED --disable-sse2)
endif()

include(ExternalProject)

ExternalProject_Add(libpostal
  GIT_REPOSITORY    https://github.com/openvenues/libpostal
  GIT_TAG           9c975972
  PATCH_COMMAND     <SOURCE_DIR>/bootstrap.sh
  CONFIGURE_COMMAND <SOURCE_DIR>/configure --prefix=<INSTALL_DIR> ${IS_SSE_DISABLED} --disable-data-download --with-pic --datadir=${POSTAL_DATA_DIR}
  BUILD_BYPRODUCTS <INSTALL_DIR>/lib/libpostal.a
)

if(POSTAL_DOWNLOAD_DATA)
  ExternalProject_Add_Step(libpostal download-data
    COMMAND <INSTALL_DIR>/bin/libpostal_data download all ${POSTAL_DATA_DIR}/libpostal
    DEPENDEES install
  )
endif()

ExternalProject_Get_Property(libpostal INSTALL_DIR)

add_dependencies(${PROJECT_NAME} libpostal)

include_directories(${INSTALL_DIR}/include)
set(LIBS ${LIBS} ${INSTALL_DIR}/lib/libpostal.a)

target_link_libraries(${PROJECT_NAME} ${LIBS})
