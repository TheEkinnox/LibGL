# Include sub directories
if (${LIBGL_BUILD_DEMO})
  add_subdirectory(Core)
  add_subdirectory(Entities)
  add_subdirectory(Physics)
  add_subdirectory(Rendering)
  add_subdirectory(App)
  add_subdirectory(Demo)

  # Propagate the Demo's project name to the parent
  set(EXE_NAME ${EXE_NAME} PARENT_SCOPE)
else()
  # Merge and cache the externally accessible include directories
  option(LIBGL_USE_APP "Add the App library to the merged target" ON)
  option(LIBGL_USE_ENTITIES "Add the Entities library to the merged target" ON)
  option(LIBGL_USE_PHYSICS "Add the Physics library to the merged target (having this forces LIBGL_USE_ENTITIES)" ON)
  option(LIBGL_USE_RENDERING "Add the Rendering library to the merged target (having this forces LIBGL_USE_PHYSICS)" ON)

  set(LIBGL_USE_PHYSICS ${LIBGL_USE_PHYSICS} OR ${LIBGL_USE_RENDERING})

  set(LIBGL_USE_ENTITIES ${LIBGL_USE_ENTITIES} OR ${LIBGL_USE_PHYSICS})

  add_subdirectory(Core)
  set(LIBGL_TARGETS ${CORE_NAME})
  set(LIBGL_INCLUDE_DIRS ${CORE_INCLUDE_DIR})

  # Entities
  if (${LIBGL_USE_ENTITIES})
    add_subdirectory(Entities)
    list(APPEND LIBGL_TARGETS ${ENTITIES_NAME})
    list(APPEND LIBGL_INCLUDE_DIRS ${ENTITIES_INCLUDE_DIR})
  endif()

  # Physics
  if (${LIBGL_USE_PHYSICS})
    add_subdirectory(Physics)
    list(APPEND LIBGL_TARGETS ${PHYSICS_NAME})
    list(APPEND LIBGL_INCLUDE_DIRS ${PHYSICS_INCLUDE_DIR})
  endif()

  # Renderer
  if (${LIBGL_USE_RENDERING})
    add_subdirectory(Rendering)
    list(APPEND LIBGL_TARGETS ${RENDERING_NAME})
    list(APPEND LIBGL_INCLUDE_DIRS ${RENDERING_INCLUDE_DIR})
  endif()

  # APP
  if (${LIBGL_USE_APP})
    add_subdirectory(App)
    list(APPEND LIBGL_TARGETS ${APP_NAME})
    list(APPEND LIBGL_INCLUDE_DIRS ${APP_INCLUDE_DIR})
  endif()

  message(STATUS "LIBGL_TARGETS: ${LIBGL_TARGETS}")
  message(STATUS "LIBGL_INCLUDE_DIRS: ${LIBGL_INCLUDE_DIRS}")

  set(LIBGL_TARGETS ${LIBGL_TARGETS} CACHE INTERNAL "")
  set(LIBGL_INCLUDE_DIRS ${LIBGL_INCLUDE_DIRS} CACHE INTERNAL "")
endif()