project( OptiMoves )

cmake_minimum_required( VERSION 3.22 )

file( GLOB
        SOURCES
        src/*.cpp )
file( GLOB
        HEADERS
        src/ )

set( PUBLIC_HEADERS
        godot-cpp/include
        godot-cpp/include/core
        godot-cpp/include/gen
        godot-cpp/godot-headers )

add_library(
        OptiMoves
        SHARED
            ${SOURCES} )

target_include_directories(
        OptiMoves
        PRIVATE
            ${HEADERS}
        PUBLIC
            ${PUBLIC_HEADERS} )

target_link_directories(
        OptiMoves
        PUBLIC
            godot-cpp/bin/ )

if( WIN32 )
    set( GODOT_LIB libgodot-cpp.windows.release.64.lib )
elseif( UNIX AND NOT APPLE )
    set( GODOT_LIB libgodot-cpp.linux.release.64.a )
endif()

target_link_libraries(
        OptiMoves
            PUBLIC
            ${GODOT_LIB} )