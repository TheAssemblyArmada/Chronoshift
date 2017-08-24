cmake_minimum_required(VERSION 2.8.11)

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "No build type selected, default to Debug")
    set(CMAKE_BUILD_TYPE "Debug")
endif()

if(MSVC)
    set(MSVC_INCREMENTAL_DEFAULT ON)
endif()

set(PROJECT rapp)

project(${PROJECT})

if(MSVC)
    string(REPLACE "INCREMENTAL" "INCREMENTAL:NO" replacementFlags ${CMAKE_EXE_LINKER_FLAGS_DEBUG})
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "/DYNAMICBASE:NO /NXCOMPAT:NO /INCREMENTAL:NO ${replacementFlags}")
    set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "/DYNAMICBASE:NO /NXCOMPAT:NO /INCREMENTAL:NO ${replacementFlags}")

    string(REPLACE "INCREMENTAL" "INCREMENTAL:NO" replacementFlags ${CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO})
    set(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO "/INCREMENTAL:NO ${replacementFlags}")
    set(CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO "/INCREMENTAL:NO ${replacementFlags}")
endif()

if(WATCOM)
    # Removes the -br flag, similar to using -static-libgcc and -static-libstdc++ with GCC
    # Removes the -xs flag, similar to using -fno-exceptions with GCC
    foreach(flag_var
        CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
        CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
        message("Processing flags ${flag_var}")
        string(REGEX REPLACE "-br[^ ]*" "" ${flag_var} "${${flag_var}}")
        string(REGEX REPLACE "-xs[^ ]*" "" ${flag_var} "${${flag_var}}")
    endforeach(flag_var)
    
    # Set packing and alignment to 1 byte to match original game ABI
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -zp1")
endif()
    
# We don't support in tree builds, so help people make the right choice.
if (CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR)
    message(FATAL_ERROR "Building in-source is not supported! Create a build dir and remove ${CMAKE_SOURCE_DIR}/CMakeCache.txt")
endif()

# This doesn't really work yet, work ongoing to make it usable
option(STANDALONE "Build a standalone version." OFF)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${rapp_SOURCE_DIR}/cmake/modules)

# Go lean and mean on windows.
if(WIN32 OR "${CMAKE_SYSTEM}" MATCHES "Windows")
    add_definitions(-DWIN32_LEAN_AND_MEAN)
    add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
    add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
    if(CMAKE_SIZEOF_VOID_P EQUAL 4)
        add_definitions(-D_USE_32BIT_TIME_T)	#This is for ABI compatibility with a few functions, remove when original binary no longer required.
    endif()
endif()

# Prevent lib prefix when built with MinGW to target windows and move to own dir.
if(MINGW)
    set(CMAKE_SHARED_LIBRARY_PREFIX "")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${rapp_BINARY_DIR}/bin)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${rapp_BINARY_DIR}/bin)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++ -std=gnu++0x -mabi=ms -Wno-invalid-offsetof")
endif()

# Enable debug features in MSVC Debug configs.
if(MSVC)
    #set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /DGAME_DEBUG_LOG")
    #set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /DGAME_DEBUG_LOG")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /DGAME_DEBUG_LOG")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DGAME_DEBUG_LOG")
else()
    add_definitions(-DGAME_DEBUG_LOG)
    add_definitions(-D__STDC_FORMAT_MACROS)
    #if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    #    add_definitions(-DGAME_DEBUG_LOG)
    #endif() 
endif()

# Locate git binary to provide information to the build environment
find_package(Git)

if(GIT_FOUND)
    # Define current git branch.
    execute_process(
        COMMAND ${GIT_EXECUTABLE} rev-parse --abbrev-ref HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE RAPP_BRANCH
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )
    add_definitions(-DRAPP_BRANCH="${RAPP_BRANCH}")

    # Define commit hash.
    execute_process(
        COMMAND ${GIT_EXECUTABLE} rev-parse HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE RAPP_COMMIT_SHA1
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )
    add_definitions(-DRAPP_COMMIT_SHA1="${RAPP_COMMIT_SHA1}")

    # Define short commit hash.
    execute_process(
        COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE RAPP_COMMIT_SHA1_SHORT
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )
    add_definitions(-DRAPP_COMMIT_SHA1_SHORT="${RAPP_COMMIT_SHA1_SHORT}")

    # Define commit count.
    execute_process(
        COMMAND ${GIT_EXECUTABLE} rev-list HEAD --count
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE RAPP_COMMIT_COUNT
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )
    add_definitions(-DRAPP_COMMIT_COUNT=${RAPP_COMMIT_COUNT})

else()
    add_definitions(-DRAPP_BRANCH="NOGITBRANCH")
    add_definitions(-DRAPP_COMMIT_SHA1="NOGITHASH")
    add_definitions(-DRAPP_COMMIT_SHA1_SHORT="NOGITHASH")
    add_definitions(-DRAPP_COMMIT_COUNT=0)
endif()

if(STANDALONE)
    add_definitions(-DRAPP_STANDALONE)
endif()

# Set where the build results will end up
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Build the launcher
add_subdirectory(launcher)

# Build RAPP
add_subdirectory(src)