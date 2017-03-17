cmake_minimum_required(VERSION 2.8)
project(Chip8Dev CXX)

set (CMAKE_CXX_STANDARD 11)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
	string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
	set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR} )
	set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR} )
	set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR} )
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )

include_directories(src src/emulator)

file(GLOB chip8 src/*.cpp src/*.h*)
file(GLOB chip8asm src/asm/*)
file(GLOB chip8emu src/emulator/*)

add_library(chip8 ${chip8})
add_executable(chip8emu ${chip8emu})
target_link_libraries(chip8emu chip8)
add_executable(chip8asm ${chip8asm})
target_link_libraries(chip8asm chip8)