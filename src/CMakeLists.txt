set(GAMEENGINE_INCLUDES
    game
    game/audio
    game/common
    game/crypto
    game/debug
    game/engine
    game/gfx
    game/io
    game/memory
    game/net
    game/platform
    game/ui
    game/util
)

set(HOOKER_SRC hooker/dllmain.cpp hooker/hooker.cpp)

set(GAMEENGINE_SRC
    game/main.cpp
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
    game/common/pcxrle.cpp
    game/common/random.cpp
    game/common/readline.cpp
    game/common/remap.cpp
    game/common/timer.cpp
    game/common/ttimer.cpp
    game/common/vector.cpp
    game/common/xordelta.cpp
    game/crypto/blowfish.cpp
    game/crypto/mpmath.cpp
    game/crypto/pk.cpp
    game/crypto/sha.cpp
    game/debug/gameassert.cpp
    game/debug/gamelogging.cpp
    game/engine/abstract.cpp
    game/engine/abstracttype.cpp
    game/engine/action.cpp
    game/engine/aircraft.cpp
    game/engine/aircrafttype.cpp
    game/engine/anim.cpp
    game/engine/animtype.cpp
    game/engine/armor.cpp
    game/engine/building.cpp
    game/engine/buildingtype.cpp
    game/engine/bullettype.cpp
    game/engine/callback.cpp
    game/engine/cargo.cpp
    game/engine/cell.cpp
    game/engine/cmdline.cpp
    game/engine/coord.cpp
    game/engine/crate.cpp
    game/engine/difficulty.cpp
    game/engine/display.cpp
    game/engine/door.cpp
    game/engine/drive.cpp
    game/engine/expansion.cpp
    game/engine/facing.cpp
    game/engine/factory.cpp
    game/engine/flasher.cpp
    game/engine/focus.cpp
    game/engine/foot.cpp
    game/engine/fuse.cpp
    game/engine/gameevent.cpp
    game/engine/gameini.cpp
    game/engine/gamekeyboard.cpp
    game/engine/gameloop.cpp
    game/engine/gamemain.cpp
    game/engine/gameoptions.cpp
    game/engine/gameptr.cpp
    game/engine/getcd.cpp
    game/engine/gmouse.cpp
    game/engine/ground.cpp
    game/engine/gscreen.cpp
    game/engine/heap.cpp
    game/engine/help.cpp
    game/engine/house.cpp
    game/engine/housestatic.cpp
    game/engine/housetype.cpp
    game/engine/infantry.cpp
    game/engine/infantrytype.cpp
    game/engine/init.cpp
    game/engine/iomap.cpp
    game/engine/land.cpp
    game/engine/layer.cpp
    game/engine/lists.cpp
    game/engine/logic.cpp
    game/engine/loaddialog.cpp
    game/engine/map.cpp
    game/engine/mission.cpp
    game/engine/multimission.cpp
    game/engine/missioncontrol.cpp
    game/engine/movie.cpp
    game/engine/msglist.cpp
    game/engine/mzone.cpp
    game/engine/object.cpp
    game/engine/objecttype.cpp
    game/engine/options.cpp
    game/engine/overlaytype.cpp
    game/engine/picture.cpp
    game/engine/power.cpp
    game/engine/queue.cpp
    game/engine/radio.cpp
    game/engine/radar.cpp
    game/engine/rtti.cpp
    game/engine/rules.cpp
    game/engine/saveload.cpp
    game/engine/scroll.cpp
    game/engine/scenario.cpp
    game/engine/session.cpp
    game/engine/smudgetype.cpp
    game/engine/source.cpp
    game/engine/sidebar.cpp
    game/engine/special.cpp
    game/engine/super.cpp
    game/engine/tab.cpp
    game/engine/target.cpp
    game/engine/techno.cpp
    game/engine/technotype.cpp
    game/engine/team.cpp
    game/engine/teamtype.cpp
    game/engine/templatetype.cpp
    game/engine/terraintype.cpp
    game/engine/theater.cpp
    game/engine/theme.cpp
    game/engine/tracker.cpp
    game/engine/trigger.cpp
    game/engine/triggertype.cpp
    game/engine/unit.cpp
    game/engine/unittype.cpp
    game/engine/utracker.cpp
    game/engine/version.cpp
    game/engine/vessel.cpp
    game/engine/vesseltype.cpp
    game/engine/voc.cpp
    game/engine/vortex.cpp
    game/engine/vox.cpp
    game/engine/warheadtype.cpp
    game/engine/weapontype.cpp
    game/gfx/blitters.cpp
    game/gfx/drawshape.cpp
    game/gfx/fading.cpp
    game/gfx/gbuffer.cpp
    game/gfx/hsv.cpp
    game/gfx/interpolate.cpp
    game/gfx/mouseshape.cpp
    game/gfx/pal.cpp
    game/gfx/palette.cpp
    game/gfx/pcx.cpp
    game/gfx/rgb.cpp
    game/gfx/shape.cpp
    game/gfx/surfacemonitor.cpp
    game/gfx/textprint.cpp
    game/gfx/tileset.cpp
    game/gfx/wsa.cpp
    game/io/b64pipe.cpp
    game/io/b64straw.cpp
    game/io/bfiofile.cpp
    game/io/blowpipe.cpp
    game/io/blowstraw.cpp
    game/io/buffpipe.cpp
    game/io/buffstraw.cpp
    game/io/cd.cpp
    game/io/cdfile.cpp
    game/io/fileclass.cpp
    game/io/filepipe.cpp
    game/io/filestraw.cpp
    game/io/gamefile.cpp
    game/io/lcwpipe.cpp
    game/io/lcwstraw.cpp
    game/io/lzopipe.cpp
    game/io/lzostraw.cpp
    game/io/mixfile.cpp
    game/io/pipe.cpp
    game/io/pcxpipe.cpp
    game/io/pcxstraw.cpp
    game/io/pkpipe.cpp
    game/io/pkstraw.cpp
    game/io/rawfile.cpp
    game/io/ramfile.cpp
    game/io/rndstraw.cpp
    game/io/shapipe.cpp
    game/io/shastraw.cpp
    game/io/straw.cpp
    game/memory/alloc.cpp
    game/memory/newdel.cpp
    game/net/connection.cpp
    game/net/commbuff.cpp
    game/net/field.cpp
    game/net/ipx.cpp
    game/net/ipxaddress.cpp
    game/net/udp.cpp
    game/net/wsock.cpp
    game/platform/eventhandler.cpp
    game/platform/initvideo.cpp
    game/platform/keyboard.cpp
    game/platform/mouse.cpp
    game/ui/bigcheck.cpp
    game/ui/checklist.cpp
    game/ui/checkbox.cpp
    game/ui/colrlist.cpp
    game/ui/controlc.cpp
    game/ui/dialog.cpp
    game/ui/droplist.cpp
    game/ui/edit.cpp
    game/ui/gadget.cpp
    game/ui/gauge.cpp
    game/ui/link.cpp
    game/ui/list.cpp
    game/ui/musiclist.cpp
    game/ui/slider.cpp
    game/ui/shapebtn.cpp
    game/ui/staticbtn.cpp
    game/ui/textbtn.cpp
    game/ui/txtlabel.cpp
    game/ui/toggle.cpp
    game/ui/msgbox.cpp
)


if(CMAKE_CONFIGURATION_TYPES)
    # Glob all the header files together to add to the project for msvc/xcode.
    # Not ideal as CMake won't notice if you add any until something else prompts a CMake run
    # but this is largely a convenience feature for MSVC/XCode users.
    file(GLOB_RECURSE GAMEENGINE_HEADERS "*.h")
    set(_GAMEENGINE_SOURCES ${GAMEENGINE_SRC})

    if(NOT STANDALONE)
        list(APPEND _GAMEENGINE_SOURCES ${HOOKER_SRC})
    endif()

    # Filter out headers from included 3rd party libraries.
    list(FILTER GAMEENGINE_HEADERS EXCLUDE REGEX "libs/.*")
    list(FILTER GAMEENGINE_HEADERS EXCLUDE REGEX "base/.*")

    foreach(_GAMEENGINE_SOURCE IN ITEMS ${_GAMEENGINE_SOURCES})
        get_filename_component(_GAMEENGINE_SOURCE_PATH "${_GAMEENGINE_SOURCE}" PATH)
        string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" _GAMEENGINE_GROUP_PATH "${_GAMEENGINE_SOURCE_PATH}")
        string(REPLACE "/" "\\" _GAMEENGINE_GROUP_PATH "${_GAMEENGINE_GROUP_PATH}")
        source_group("Source Files\\${_GAMEENGINE_GROUP_PATH}" FILES "${_GAMEENGINE_SOURCE}")
    endforeach()

    foreach(_GAMEENGINE_HEADER IN ITEMS ${GAMEENGINE_HEADERS})
        get_filename_component(_GAMEENGINE_HEADER_PATH "${_GAMEENGINE_HEADER}" PATH)
        string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" _GAMEENGINE_HGROUP_PATH "${_GAMEENGINE_HEADER_PATH}")
        string(REPLACE "/" "\\" _GAMEENGINE_HGROUP_PATH "${_GAMEENGINE_HGROUP_PATH}")
        source_group("Header Files\\${_GAMEENGINE_HGROUP_PATH}" FILES "${_GAMEENGINE_HEADER}")
    endforeach()
endif()

# Build and link the EXE or DLL.
if(STANDALONE)
    add_executable(chronoshift ${GAMEENGINE_SRC} ${GAMEENGINE_HEADERS})
    target_include_directories(chronoshift PUBLIC ${GAMEENGINE_INCLUDES})
else()
    add_library(chronoshift SHARED ${HOOKER_SRC} ${GAMEENGINE_SRC} ${GAMEENGINE_HEADERS})
    target_include_directories(chronoshift BEFORE PUBLIC hooker ${GAMEENGINE_INCLUDES})
endif()

if(WIN32 OR "${CMAKE_SYSTEM}" MATCHES "Windows")
    target_link_libraries(chronoshift winmm)

    if(STANDALONE)
        target_link_libraries(chronoshift ws2_32)
    else()
        target_link_libraries(chronoshift wsock32) # Original links wsock32, not ws2_32
    endif()
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
    target_link_libraries(chronoshift rt)
endif()

target_compile_definitions(chronoshift PUBLIC $<$<CONFIG:DEBUG>:CHRONOSHIFT_DEBUG=1>)

if(STANDALONE)
    target_compile_definitions(chronoshift PUBLIC CHRONOSHIFT_STANDALONE=1)
endif()

# Enable using bools in place of bitfields for boolean class members.
# This is for matching the ABI of the original game.
if(NO_BITFIELDS)
    target_compile_definitions(chronoshift PUBLIC CHRONOSHIFT_NO_BITFIELDS=1)
endif()

# Enable Thyme debug logging.
if(LOGGING)
    target_compile_definitions(chronoshift PUBLIC CHRONOSHIFT_LOGGING=1)
endif()

# Enable Thyme debug assertions.
if(ASSERTIONS)
    target_compile_definitions(chronoshift PUBLIC CHRONOSHIFT_ASSERTS=1)
endif()

target_link_libraries(chronoshift base)

if(DDRAW_FOUND)
    target_compile_definitions(chronoshift PRIVATE BUILD_WITH_DDRAW=1)
    target_link_libraries(chronoshift DirectX::ddraw)
endif()

if(DSOUND_FOUND)
    target_compile_definitions(chronoshift PRIVATE BUILD_WITH_DSOUND=1)
    target_link_libraries(chronoshift DirectX::dsound)
endif()

if(SDL2_FOUND)
    target_compile_definitions(chronoshift PRIVATE BUILD_WITH_SDL2=1)
    target_link_libraries(chronoshift SDL2::SDL2)
endif()

if(OPENAL_FOUND)
    target_compile_definitions(chronoshift PRIVATE BUILD_WITH_OPENAL=1)
    target_link_libraries(chronoshift ${OPENAL_LIBRARIES})
    target_include_directories(chronoshift PUBLIC ${OPENAL_INCLUDE_DIRS})
endif()

if(LZO_FOUND)
    target_link_libraries(chronoshift ${LZO_LIBRARIES})
    target_include_directories(chronoshift PUBLIC ${LZO_INCLUDE_DIRS})
else()
    message("Prebuilt LZO library not found, using minilzo.")
    add_library(minilzo STATIC EXCLUDE_FROM_ALL ${CMAKE_SOURCE_DIR}/libs/minilzo/minilzo.c)
    target_include_directories(minilzo PUBLIC ${CMAKE_SOURCE_DIR}/libs)
    target_compile_definitions(chronoshift PRIVATE CHRONOSHIFT_USE_MINILZO=1)
    target_link_libraries(chronoshift minilzo)
endif()
