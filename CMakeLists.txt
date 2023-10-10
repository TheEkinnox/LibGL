cmake_minimum_required(VERSION 3.23 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(LIBGL_ASSETS_DIR_NAME assets)
set(LIBGL_ASSETS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${LIBGL_ASSETS_DIR_NAME})

option(LIBGL_BUILD_DEMO "Build Demo executable when on, don't when off" ON)

project(LibGL)

add_subdirectory(dependencies)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_FOLDER LibGL)

add_subdirectory(source)

if (MSVC AND ${LIBGL_BUILD_DEMO})
  set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${EXE_NAME})
endif()