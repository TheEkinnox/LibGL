cmake_minimum_required(VERSION 3.23 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(GLAD_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/GLAD/src/glad.cpp)
set(GLAD_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/GLAD/include)
set(GLFW_NAME ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/GLFW/lib-vc2019/glfw3.lib)
set(GLFW_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/GLFW/include)
set(STB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/STB)

project(Modern_OpenGL_Loick)

add_subdirectory(source)

if (MSVC)
  set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT App)
endif()