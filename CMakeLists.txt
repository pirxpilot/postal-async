cmake_minimum_required(VERSION 2.8)

set(BUILD_SHARED_LIBS ON CACHE BOOL "Build shared libraries")

include(FindPkgConfig)

# Name of the project (will be the name of the plugin)
project(postal-async)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  add_definitions(-fno-builtin-memcmp)
endif()

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

pkg_check_modules(POSTAL REQUIRED libpostal)
include_directories(${POSTAL_INCLUDE_DIRS})
set(LIBS ${LIBS} ${POSTAL_LIBRARIES})

target_link_libraries(${PROJECT_NAME} ${LIBS})
