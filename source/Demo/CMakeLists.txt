# set project
get_filename_component(CURRENT_FOLDER_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
set(PROJECT_NAME ${CURRENT_FOLDER_NAME})


###############################
#                             #
# Sources                     #
#                             #
###############################

# Add source files
file(GLOB_RECURSE SOURCE_FILES 
	${CMAKE_CURRENT_SOURCE_DIR}/*.c
	${CMAKE_CURRENT_SOURCE_DIR}/*.cc # C with classe
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/*.cxx
	${CMAKE_CURRENT_SOURCE_DIR}/*.c++)
	
# Add header files
set(PROJECT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE HEADER_FILES 
	${CMAKE_CURRENT_SOURCE_DIR}/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/*.inl)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${PROJECT_FILES})

file(GLOB_RECURSE ASSETS_FILES
	${ASSETS_DIR}/shaders/*
	${ASSETS_DIR}/textures/*)

source_group(TREE ${ASSETS_DIR} PREFIX Assets FILES ${ASSETS_FILES})

set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${ASSETS_FILES})


###############################
#                             #
# Executable                  #
#                             #
###############################

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES} ${ASSETS_FILES})

include_directories(${PROJECT_INCLUDE_DIR}
    ${LIBMATH_INCLUDE_DIR}
    ${CORE_INCLUDE_DIR} ${PHYSICS_INCLUDE_DIR} ${ENTITIES_INCLUDE_DIR} ${RENDERING_INCLUDE_DIR} ${APP_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME}
    PRIVATE
	${LIBMATH_NAME}
	${CORE_NAME}
	${PHYSICS_NAME}
	${ENTITIES_NAME}
	${RENDERING_NAME}
	${APP_NAME})

if(MSVC)
  set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
  target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
  target_link_options(${PROJECT_NAME} PRIVATE /FORCE:UNRESOLVED)
else()
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

# copy the necessary dlls in the build directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}> $<TARGET_FILE_DIR:${PROJECT_NAME}>
  COMMAND_EXPAND_LISTS
)

# remove the existing assets folder from the build directory (ensures removes are also replicated)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E rm -rf $<TARGET_FILE_DIR:${PROJECT_NAME}>/${ASSETS_DIR_NAME})

# copy the assets folder in the target build directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${ASSETS_DIR} $<TARGET_FILE_DIR:${PROJECT_NAME}>/${ASSETS_DIR_NAME})

set(EXE_NAME ${PROJECT_NAME} PARENT_SCOPE)