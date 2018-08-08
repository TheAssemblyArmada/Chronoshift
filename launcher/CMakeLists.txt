# Build and link the launcher.
set(LAUNCHER_SRC launcher.cpp resource.rc)
add_executable(launch_chronoshift ${LAUNCHER_SRC})
target_link_libraries(launch_chronoshift)
