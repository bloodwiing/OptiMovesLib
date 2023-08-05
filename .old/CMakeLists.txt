cmake_minimum_required( VERSION 3.21 )

project( NodeUtils )

set( CMAKE_CXX_COMPILER g++ )
set( CMAKE_CXX_STANDARD 14 )
SET( CMAKE_CXX_FLAGS "-g -O3" )

file( GLOB SOURCE src/**.cpp )

add_executable( NodeUtils ${SOURCE} )

target_include_directories( NodeUtils
        PUBLIC
            godot-cpp/include
            godot-cpp/include/core
            godot-cpp/include/gen
            godot-cpp/godot-headers )

target_link_directories( NodeUtils
        PUBLIC
            godot-cpp/bin )
target_link_libraries( NodeUtils
        PUBLIC
            godot-cpp.windows.debug.64 )
