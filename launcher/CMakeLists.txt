# Build and link the launcher.
set(LAUNCHER_SRC launcher.cpp resource.rc)
add_executable(launchrapp ${LAUNCHER_SRC})
target_link_libraries(launchrapp)
