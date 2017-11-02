# Add platform libraries needed for the build
if(WIN32 OR "${CMAKE_SYSTEM}" MATCHES "Windows")
    find_package(DirectX)
    
    if(DirectX_DDRAW_FOUND)
        message("Located DirectDraw.")
        set(PLATFORM_INCLUDES ${PLATFORM_INCLUDES} ${DirectX_DDRAW_INCLUDE_DIR})
        set(PLATFORM_LIBS ${PLATFORM_LIBS} ${DirectX_DDRAW_LIBRARY})
    endif()
    
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
    game/engine
    game/gfx
    game/io
    game/memory
    game/platform
    game/ui
)

set(HOOKER_SRC hooker/dllmain.cpp hooker/hooker.cpp)

set(GAMEENGINE_SRC
    game/gamedebug.cpp
    game/startup.cpp
    game/common/base64.cpp
    game/common/bitarray.cpp
    game/common/buffer.cpp
    game/common/cpudetect.cpp
    game/common/crc.cpp
    game/common/critsection.cpp
    game/common/fixed.cpp
    game/common/dipthong.cpp
    game/common/globals.cpp
    game/common/ini.cpp
    game/common/language.cpp
    game/common/lcw.cpp
    game/common/ostimer.cpp
    game/common/random.cpp
    game/common/readline.cpp
    game/common/remap.cpp
    game/common/timer.cpp
    game/common/ttimer.cpp
    game/common/vector.cpp
    game/common/wwstring.cpp
    game/common/xordelta.cpp
    game/crypto/blowfish.cpp
    game/crypto/pk.cpp
    game/crypto/mpmath.cpp
    game/crypto/sha.cpp
    game/engine/utracker.cpp
    game/gfx/blitters.cpp
    game/gfx/drawshape.cpp
    game/gfx/fading.cpp
    game/gfx/gbuffer.cpp
    game/gfx/hsv.cpp
    game/gfx/interpolate.cpp
    game/gfx/mouseshape.cpp
    game/gfx/pal.cpp
    game/gfx/palette.cpp
    game/gfx/rgb.cpp
    game/gfx/shape.cpp
    game/gfx/textprint.cpp
    game/gfx/surfacemonitor.cpp
    game/gfx/wsa.cpp
    game/io/b64pipe.cpp
    game/io/b64straw.cpp
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
    game/io/lcwpipe.cpp
    game/io/lcwstraw.cpp
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
    game/net/field.cpp
    game/platform/keyboard.cpp
    game/platform/mouse.cpp
    game/ui/bigcheck.cpp
    game/ui/checklist.cpp
    game/ui/checkbox.cpp
    game/ui/controlc.cpp
    game/ui/dialog.cpp
    game/ui/droplist.cpp
    game/ui/edit.cpp
    game/ui/gadget.cpp
    game/ui/gauge.cpp
    game/ui/link.cpp
    game/ui/list.cpp
    game/ui/slider.cpp
    game/ui/shapebtn.cpp
    game/ui/textbtn.cpp
    game/ui/toggle.cpp
)

if(STANDALONE)
    add_executable(rapp ${GAMEENGINE_SRC})
    target_include_directories(rapp BEFORE PUBLIC ${PLATFORM_INCLUDES} ${GAMEENGINE_INCLUDES})
else()
    add_library(rapp SHARED ${HOOKER_SRC} ${GAMEENGINE_SRC})
    target_include_directories(rapp BEFORE PUBLIC ${PLATFORM_INCLUDES} ${GAMEENGINE_INCLUDES} hooker)
endif()

target_link_libraries(rapp ${PLATFORM_LIBS})
