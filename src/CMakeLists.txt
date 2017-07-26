# Add platform libraries needed for the build
if(WIN32 OR "${CMAKE_SYSTEM}" MATCHES "Windows")
    #set(PLATFORM_LIBS ${PLATFORM_LIBS} winmm)
else()
    #No other platforms yet.
endif()

# Build and link the DLL.
set(GAMEENGINE_INCLUDES
    base
    game
)

set(HOOKER_SRC hooker/dllmain.cpp hooker/hooker.cpp)

set(GAMEENGINE_SRC
    game/alloc.cpp
    game/critsection.cpp
    game/gamedebug.cpp
    game/newdel.cpp
)

add_library(rapp SHARED ${HOOKER_SRC} ${GAMEENGINE_SRC})

target_include_directories(rapp BEFORE PUBLIC ${PLATFORM_INCLUDES} hooker ${GAMEENGINE_INCLUDES})
#set_target_properties(rapp PROPERTIES LINK_FLAGS "option start=DllMain")

target_link_libraries(rapp ${PLATFORM_LIBS})