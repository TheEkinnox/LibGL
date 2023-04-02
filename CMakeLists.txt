cmake_minimum_required(VERSION 3.23 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(ASSETS_DIR_NAME assets)
set(ASSETS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${ASSETS_DIR_NAME})

project(Modern_OpenGL_Loick)

option(BUILD_AS_LIBRARY "Build App as lib when set, build as exe when not set" ON)

add_subdirectory(dependencies)
add_subdirectory(source)

if (MSVC AND NOT ${BUILD_AS_LIBRARY})
  set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${APP_NAME})
endif()