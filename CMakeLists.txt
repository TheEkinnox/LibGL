cmake_minimum_required(VERSION 3.23 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(ASSETS_DIR_NAME assets)
set(ASSETS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${ASSETS_DIR_NAME})

project(ModernOpenGL)

option(LIBGL_BUILD_EXE "Build App executable when on, don't when off" ON)

if (NOT ${LIBGL_BUILD_EXE})
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)
  set(CMAKE_FOLDER LibGL)
endif()

add_subdirectory(dependencies)
add_subdirectory(source)

if (MSVC AND ${LIBGL_BUILD_EXE})
  set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${APP_NAME})
endif()