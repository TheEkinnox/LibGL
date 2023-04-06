# Download GLFW
include(FetchContent)
FetchContent_Declare(
	GLFW 
	GIT_REPOSITORY https://github.com/glfw/glfw.git
	GIT_TAG 3.3.8
	GIT_SHALLOW ON
)
set(BUILD_SHARED_LIBS ON)
set(GLFW_INSTALL OFF)
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)

FetchContent_MakeAvailable(GLFW)

# Download LibMath
FetchContent_Declare(
	LibMath
	GIT_REPOSITORY ssh://git@git.isartintra.com:2424/2022/Lib_De_Math/Loick.git
	GIT_TAG origin/main
	GIT_SHALLOW ON
)
set(BUILD_SHARED_LIBS OFF)
set(LIBMATH_BUILD_TESTS OFF)

FetchContent_MakeAvailable(LibMath)

add_subdirectory(glad)

# Set shared variables
set(STB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/STB PARENT_SCOPE)

set(GLAD_NAME ${GLAD_NAME} PARENT_SCOPE)
set(GLAD_INCLUDE_DIR ${GLAD_INCLUDE_DIR} PARENT_SCOPE)

set(GLFW_NAME glfw PARENT_SCOPE)
set(GLFW_INCLUDE_DIR ${GLFW_SOURCE_DIR}/include PARENT_SCOPE)

set(LIBMATH_NAME LibMath PARENT_SCOPE)
set(LIBMATH_INCLUDE_DIR ${LibMath_SOURCE_DIR}/LibMath/Header PARENT_SCOPE)