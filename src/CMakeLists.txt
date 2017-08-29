# Add platform libraries needed for the build
if(WIN32 OR "${CMAKE_SYSTEM}" MATCHES "Windows")
    set(PLATFORM_LIBS ${PLATFORM_LIBS} winmm)
else()
    #No other platforms yet.
endif()

# Build and link the DLL.
set(GAMEENGINE_INCLUDES
    base
    game
    game/common
    game/crypto
    game/io
    game/memory
)

set(HOOKER_SRC hooker/dllmain.cpp hooker/hooker.cpp)

set(GAMEENGINE_SRC
    game/gamedebug.cpp
    game/startup.cpp
    game/common/buffer.cpp
    game/common/cpudetect.cpp
    game/common/crc.cpp
    game/common/critsection.cpp
    game/common/fixed.cpp
    game/common/random.cpp
    game/common/readline.cpp
    game/common/wwstring.cpp
    game/crypto/blowfish.cpp
    game/crypto/pk.cpp
    game/crypto/mpmath.cpp
    game/crypto/sha.cpp
    game/io/bfiofileclass.cpp
    game/io/blowpipe.cpp
    game/io/blowstraw.cpp
    game/io/buffpipe.cpp
    game/io/buffstraw.cpp
    game/io/ccfileclass.cpp
    game/io/cd.cpp
    game/io/cdfileclass.cpp
    game/io/fileclass.cpp
    game/io/filepipe.cpp
    game/io/filestraw.cpp
    game/io/mixfile.cpp
    game/io/pipe.cpp
    game/io/pkpipe.cpp
    game/io/pkstraw.cpp
    game/io/rawfileclass.cpp
    game/io/rndstraw.cpp
    game/io/shapipe.cpp
    game/io/shastraw.cpp
    game/io/straw.cpp
    game/memory/alloc.cpp
    game/memory/newdel.cpp
)

if(STANDALONE)
    add_executable(rapp ${GAMEENGINE_SRC})
    target_include_directories(rapp BEFORE PUBLIC ${PLATFORM_INCLUDES} ${GAMEENGINE_INCLUDES})
else()
    add_library(rapp SHARED ${HOOKER_SRC} ${GAMEENGINE_SRC})
    target_include_directories(rapp BEFORE PUBLIC ${PLATFORM_INCLUDES} hooker ${GAMEENGINE_INCLUDES})
endif()

target_link_libraries(rapp ${PLATFORM_LIBS})
