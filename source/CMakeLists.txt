# Include sub directories
add_subdirectory(Core)
add_subdirectory(Physics)
add_subdirectory(Resources)
add_subdirectory(LowRenderer)
add_subdirectory(App)

# Propagate the App's project name to the parent
set(APP_NAME ${APP_NAME} PARENT_SCOPE)