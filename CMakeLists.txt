cmake_minimum_required(VERSION 3.1.0)

if(POLICY CMP0074)
    cmake_policy(SET CMP0074 NEW)
endif()

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "No build type selected, default to Debug")
    set(CMAKE_BUILD_TYPE "Debug")
endif()

if(MSVC)
    set(MSVC_INCREMENTAL_DEFAULT ON)
endif()

project(chronoshift VERSION 3.3 LANGUAGES C CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/modules)
set(CMAKE_NO_SYSTEM_FROM_IMPORTED TRUE) # Needed to prevent FindDirectX screwing up a mingw build.

if(MSVC)
    # Prevent incremental linking.
    string(REPLACE "INCREMENTAL" "INCREMENTAL:NO" replacementFlags ${CMAKE_EXE_LINKER_FLAGS_DEBUG})
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "/DYNAMICBASE:NO /NXCOMPAT:NO /INCREMENTAL:NO ${replacementFlags}")
    set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "/DYNAMICBASE:NO /NXCOMPAT:NO /INCREMENTAL:NO ${replacementFlags}")

    string(REPLACE "INCREMENTAL" "INCREMENTAL:NO" replacementFlags ${CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO})
    set(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO "/INCREMENTAL:NO ${replacementFlags}")
    set(CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO "/INCREMENTAL:NO ${replacementFlags}")
    
    # Disable Run Time Checking.
    foreach(flag_var
        CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
        CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO
        CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
        CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO)
        #message("Processing flags ${flag_var}")
        string(REGEX REPLACE "/RTC[^ ]*" "" ${flag_var} "${${flag_var}}")
    endforeach(flag_var)
    # Set warning level 3
    # disable C4244: conversion from 'double' to 'float', possible loss of data
    # disable C4800: 'BOOL' : forcing value to bool 'true' or 'false' (performance warning)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3 /wd4244 /wd4800")
endif()

if(WATCOM)
    # Removes the -br flag, similar to using -static-libgcc and -static-libstdc++ with GCC
    # Removes the -xs flag, similar to using -fno-exceptions with GCC
    # Changes debug level to -d1 rather than -d2 to give mangled names
    foreach(flag_var
        CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
        CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO
        CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
        CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO)
        #message("Processing flags ${flag_var}")
        string(REGEX REPLACE "-br[^ ]*" "" ${flag_var} "${${flag_var}}")
        string(REGEX REPLACE "-xs[^ ]*" "" ${flag_var} "${${flag_var}}")
        string(REGEX REPLACE "-d2" "-d1" ${flag_var} "${${flag_var}}")
    endforeach(flag_var)
    
    # Set packing and alignment to 1 byte to match original game ABI with -zp1
    # Optimise for run time with -ot
    # Some stack trace option we believe the original was compiled with -of+
    # Expand intrinsics inline with -oi
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -zp1 -ot -of+ -oi")
    
    # Dump useful debug info with -db
    # Disable stack checks in debug with -s
    # Prevents merging identical function returns with -oc
    # Prevents merging identical functions into a single function -ou
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -db -s -ou -oc")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -db")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -db -s")
    set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -db")
    
    # Make watcom dump the debug symbols to a file with the .sym extension
    set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "Debug CODEVIEW All ${CMAKE_SHARED_LINKER_FLAGS_DEBUG}")
    set(CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO "opt symf ${CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO}")
    set(CMAKE_MODULE_LINKER_FLAGS_DEBUG "Debug CODEVIEW All ${CMAKE_MODULE_LINKER_FLAGS_DEBUG}")
    set(CMAKE_MODULE_LINKER_FLAGS_RELWITHDEBINFO "opt symf ${CMAKE_MODULE_LINKER_FLAGS_RELWITHDEBINFO}")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "Debug CODEVIEW All ${CMAKE_EXE_LINKER_FLAGS_DEBUG}")
    set(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO "opt symf ${CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO}")
    
    # Disable some warnings.
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -wcd=004 -wcd=248 -wcd=549")
else()
    # Require that we have at least C++11 when not using Watcom.
    set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
    set(CMAKE_CXX_STANDARD 11)
endif()

# If we exclude Watcom and MSVC we now assume GCC like compiler syntax.
if(NOT MSVC AND NOT WATCOM)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-undefined-var-template")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-varargs")
    endif()
endif()


# We don't support in tree builds, so help people make the right choice.
if (CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR)
    message(FATAL_ERROR "Building in-source is not supported! Create a build dir and remove ${CMAKE_SOURCE_DIR}/CMakeCache.txt")
endif()

if(WIN32 OR "${CMAKE_SYSTEM}" MATCHES "Windows")
    set(DEFAULT_STANDALONE OFF)
else()
    set(DEFAULT_STANDALONE ON)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(DEFAULT_LOGGING ON)
    set(DEFAULT_ASSERTIONS ON)
else()
    set(DEFAULT_ASSERTIONS OFF)
    set(DEFAULT_LOGGING OFF)
endif()

# Standalone builds still don't actually run but can be built for dev purposes.
option(STANDALONE "Build a standalone version." ${DEFAULT_STANDALONE})
option(NO_BITFIELDS "Use standard bool type in place of bitfields." OFF)
option(LOGGING "Enable debug logging." ${DEFAULT_LOGGING})
option(ASSERTIONS "Enable debug assertions." ${DEFAULT_ASSERTIONS})
option(USE_SDL2 "Enable SDL2 for graphics and input handling." OFF)
option(USE_OPENAL "Enable OpenAL for audio output." OFF)

# Adjust size of time_t for original binary abi.
if(WIN32 OR "${CMAKE_SYSTEM}" MATCHES "Windows")
    if(CMAKE_SIZEOF_VOID_P EQUAL 4 AND NOT STANDALONE)
        add_definitions(-D_USE_32BIT_TIME_T) # This is for ABI compatibility with a few functions, remove when original binary no longer required.
    endif()
endif()

# Prevent lib prefix when built with MinGW to target windows and move to own dir.
if(MINGW)
    set(CMAKE_SHARED_LIBRARY_PREFIX "")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++ -mabi=ms -Wno-invalid-offsetof -Wall")
endif()

# Check for libraries we plan to build against.
if(USE_SDL2)
    find_package(SDL2 REQUIRED)
else()
    list(APPEND DX_PACKAGES ddraw)
endif()

if(USE_OPENAL)
    find_package(OpenAL REQUIRED)
else()
    list(APPEND DX_PACKAGES dsound)
endif()

if(NOT USE_SDL2 OR NOT USE_OPENAL)
    find_package(DirectX REQUIRED ${DX_PACKAGES})
endif()

# Add LZO compression for save game support
find_package(LZO)

# Set where the build results will end up
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Set version info for the base config module
set(GITINFO_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(GITINFO_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(GITINFO_VERSION_PATCH ${PROJECT_VERSION_PATCH})

if(NOT STANDALONE)
    set(BASECONF_WINSOCK32 TRUE BOOL)
endif()

# Add base module
add_subdirectory(libs/baseconfig EXCLUDE_FROM_ALL)

if(NOT STANDALONE)
    # Build the launcher
    add_subdirectory(launcher)
endif()

# Build Chronoshift
add_subdirectory(src)
