cmake_minimum_required(VERSION 3.0.0)

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "No build type selected, default to Debug")
    set(CMAKE_BUILD_TYPE "Debug")
endif()

if(MSVC)
    set(MSVC_INCREMENTAL_DEFAULT ON)
endif()

set(PROJECT CHRONOSHIFT VERSION 3.3)

project(${PROJECT})

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
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
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
    
    # Set packing and alignment to 1 byte to match original game ABI
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -zp1")
    
    # Dump useful debug info with -db
    # Disable stack checks in debug with -s
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -db -s")
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
else()
    if (CMAKE_VERSION VERSION_LESS "3.1")
        if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
            set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
        endif ()
    else ()
        set (CMAKE_CXX_STANDARD 11)
    endif ()
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

# This doesn't really work yet, work ongoing to make it usable.
# It should still compile fine.
option(STANDALONE "Build a standalone version." ${DEFAULT_STANDALONE})
option(NO_BITFIELDS "Use standard bool type in place of bitfields." OFF)
option(LOGGING "Enable debug logging." ${DEFAULT_LOGGING})
option(ASSERTIONS "Enable debug assertions." ${DEFAULT_ASSERTIONS})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CHRONOSHIFT_SOURCE_DIR}/cmake/modules)

# Go lean and mean on windows.
if(WIN32 OR "${CMAKE_SYSTEM}" MATCHES "Windows")
    add_definitions(-DWIN32_LEAN_AND_MEAN)
    add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
    add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
    if(CMAKE_SIZEOF_VOID_P EQUAL 4 AND NOT STANDALONE)
        add_definitions(-D_USE_32BIT_TIME_T)    #This is for ABI compatibility with a few functions, remove when original binary no longer required.
    endif()
endif()

# Prevent lib prefix when built with MinGW to target windows and move to own dir.
if(MINGW)
    add_definitions(-D__STDC_FORMAT_MACROS)
    set(CMAKE_SHARED_LIBRARY_PREFIX "")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CHRONOSHIFT_BINARY_DIR}/bin)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CHRONOSHIFT_BINARY_DIR}/bin)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++ -mabi=ms -Wno-invalid-offsetof -Wall")
endif()

# Define __STDC_FORMAT_MACROS so #include <inttypes.h> works as expected.
if(WATCOM)
    add_definitions(-D__STDC_FORMAT_MACROS)
endif()

# Enable Chronoshift debug features in Debug configs.
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DCHRONOSHIFT_DEBUG")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DCHRONOSHIFT_DEBUG")
endif()

# Enable Chronoshift debug logging.
if(LOGGING)
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DCHRONOSHIFT_LOGGING")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DCHRONOSHIFT_LOGGING")
    add_definitions(-DCHRONOSHIFT_LOGGING)
endif()

# Enable Chronoshift debug assertions.
if(ASSERTIONS)
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DCHRONOSHIFT_ASSERTS")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DCHRONOSHIFT_ASSERTS")
    add_definitions(-DCHRONOSHIFT_ASSERT)
endif()

# Locate git binary to provide information to the build environment
find_package(Git)

if(GIT_FOUND)
    # Define current git branch.
    execute_process(
        COMMAND ${GIT_EXECUTABLE} rev-parse --abbrev-ref HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE CHRONOSHIFT_BRANCH
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )
    add_definitions(-DCHRONOSHIFT_BRANCH="${CHRONOSHIFT_BRANCH}")

    # Define commit hash.
    execute_process(
        COMMAND ${GIT_EXECUTABLE} rev-parse HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE CHRONOSHIFT_COMMIT_SHA1
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )
    add_definitions(-DCHRONOSHIFT_COMMIT_SHA1="${CHRONOSHIFT_COMMIT_SHA1}")

    # Define short commit hash.
    execute_process(
        COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE CHRONOSHIFT_COMMIT_SHA1_SHORT
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )
    add_definitions(-DCHRONOSHIFT_COMMIT_SHA1_SHORT="${CHRONOSHIFT_COMMIT_SHA1_SHORT}")

    # Define commit count.
    execute_process(
        COMMAND ${GIT_EXECUTABLE} rev-list HEAD --count
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE CHRONOSHIFT_COMMIT_COUNT
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )
    add_definitions(-DCHRONOSHIFT_COMMIT_COUNT=${CHRONOSHIFT_COMMIT_COUNT})

    # Define commit time stamp.
    execute_process(
        COMMAND ${GIT_EXECUTABLE} log -1 --pretty=format:%ct
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE CHRONOSHIFT_COMMIT_TSTAMP
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )
    add_definitions(-DCHRONOSHIFT_COMMIT_TSTAMP=${CHRONOSHIFT_COMMIT_TSTAMP})

    # Define commit author.
    execute_process(
        COMMAND ${GIT_EXECUTABLE} log -1 --pretty=format:'%an'
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE CHRONOSHIFT_COMMIT_AUTHOR
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )
    add_definitions(-DCHRONOSHIFT_COMMIT_AUTHOR="${CHRONOSHIFT_COMMIT_AUTHOR}")
    
    # Define commit date and time. Make builds more reproducible than using __DATE__ and __TIME__
    execute_process(
        COMMAND ${GIT_EXECUTABLE} log -1 --date=iso --pretty=format:%cd
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE CHRONOSHIFT_DATE_AND_TIME
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )
    string(REPLACE " " ";" CHRONOSHIFT_DATE_AND_TIME ${CHRONOSHIFT_DATE_AND_TIME})
    list(GET CHRONOSHIFT_DATE_AND_TIME 0 CHRONOSHIFT_DATE)
    list(GET CHRONOSHIFT_DATE_AND_TIME 1 CHRONOSHIFT_TIME)
    list(GET CHRONOSHIFT_DATE_AND_TIME 2 CHRONOSHIFTTZONE)
    add_definitions(-DCHRONOSHIFT_COMMIT_DATE="${CHRONOSHIFT_DATE}")
    add_definitions(-DCHRONOSHIFT_COMMIT_TIME="${CHRONOSHIFT_TIME} ${CHRONOSHIFT_TZONE}")

else()
    add_definitions(-DCHRONOSHIFT_BRANCH="netherrealm")
    add_definitions(-DCHRONOSHIFT_COMMIT_SHA1="DEADBEEFDEADBEEFDEADBEEFDEADBEEFDEADBEEF")
    add_definitions(-DCHRONOSHIFT_COMMIT_SHA1_SHORT="DEADBEEF")
    add_definitions(-DCHRONOSHIFT_COMMIT_COUNT=0)
    add_definitions(-DCHRONOSHIFT_COMMIT_TSTAMP=0)
    add_definitions(-DCHRONOSHIFT_COMMIT_DATE="1955-11-12")
    add_definitions(-DCHRONOSHIFT_COMMIT_TIME="22:04:00 +0000")
    add_definitions(-DCHRONOSHIFT_COMMIT_AUTHOR="Someone")
endif()

add_definitions(-DCHRONOSHIFT_MAJOR=${CHRONOSHIFT_VERSION_MAJOR})
add_definitions(-DCHRONOSHIFT_MINOR=${CHRONOSHIFT_VERSION_MINOR})

if(STANDALONE)
    add_definitions(-DCHRONOSHIFT_STANDALONE)
endif()

if(NO_BITFIELDS)
    add_definitions(-DCHRONOSHIFT_NO_BITFIELDS)
endif()

# Set where the build results will end up
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

if(NOT STANDALONE)
# Build the launcher
add_subdirectory(launcher)
endif()

# Build Chronoshift
add_subdirectory(src)
