# Include sub directories
if (${LIBGL_BUILD_EXE})
  add_subdirectory(Core)
  add_subdirectory(Physics)
  add_subdirectory(Resources)
  add_subdirectory(LowRenderer)
  add_subdirectory(App)

  # Propagate the App's project name to the parent
  set(APP_NAME ${APP_NAME} PARENT_SCOPE)
else()
  # Merge and cache the externally accessible include directories
  option(LIBGL_USE_PHYSICS "Add the Physics library to the merged target" ON)
  option(LIBGL_USE_RESOURCES "Add the Resources library to the merged target" ON)
  option(LIBGL_USE_RENDERER "Add the LowRenderer library to the merged target (having this forces LIBGL_USE_PHYSICS and LIBGL_USE_RESOURCES)" ON)

  add_subdirectory(Core)
  set(LIBGL_TARGETS ${CORE_NAME})
  set(LIBGL_INCLUDE_DIRS ${CORE_INCLUDE_DIR})

  # Physics
  if (${LIBGL_USE_PHYSICS} OR ${LIBGL_USE_RENDERER})
    add_subdirectory(Physics)
    list(APPEND LIBGL_TARGETS ${PHYSICS_NAME})
    list(APPEND LIBGL_INCLUDE_DIRS ${PHYSICS_INCLUDE_DIR})
  endif()

  # Resources
  if (${LIBGL_USE_RESOURCES} OR ${LIBGL_USE_RENDERER})
    add_subdirectory(Resources)
    list(APPEND LIBGL_TARGETS ${RESOURCES_NAME})
    list(APPEND LIBGL_INCLUDE_DIRS ${RESOURCES_INCLUDE_DIR})
  endif()

  # Renderer
  if (${LIBGL_USE_RENDERER})
    add_subdirectory(LowRenderer)
    list(APPEND LIBGL_TARGETS ${RENDERER_NAME})
    list(APPEND LIBGL_INCLUDE_DIRS ${RENDERER_INCLUDE_DIR})
  endif()

  message(STATUS "LIBGL_TARGETS: ${LIBGL_TARGETS}")
  message(STATUS "LIBGL_INCLUDE_DIRS: ${LIBGL_INCLUDE_DIRS}")

  set(LIBGL_TARGETS ${LIBGL_TARGETS} CACHE INTERNAL "")
  set(LIBGL_INCLUDE_DIRS ${LIBGL_INCLUDE_DIRS} CACHE INTERNAL "")
endif()