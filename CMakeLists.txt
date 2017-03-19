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

include_directories(src)

file(GLOB chip8 src/*.cpp src/*.h*)
file(GLOB chip8asm src/asm/*)
file(GLOB chip8disasm src/disasm/*)
file(GLOB chip8emu src/emulator/*)

add_executable(chip8emu ${chip8} ${chip8emu})
target_compile_definitions(chip8emu PRIVATE CHIP_EMU)

add_executable(chip8asm ${chip8} ${chip8asm})
target_compile_definitions(chip8asm PRIVATE CHIP_ASM)

add_executable(chip8disasm ${chip8} ${chip8disasm})
target_compile_definitions(chip8disasm PRIVATE CHIP_DISASM)
