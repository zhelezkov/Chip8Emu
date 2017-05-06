cmake_minimum_required(VERSION 3.8)
project(Chip8Dev CXX)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(PCRE REQUIRED)

set (CMAKE_CXX_STANDARD 11)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
	string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR})
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR})
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR})
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )


file(GLOB chip8 src/*.cpp src/*.h*)
file(GLOB chip8asm src/asm/*)
file(GLOB chip8disasm src/disasm/*)
file(GLOB chip8emu src/emulator/*)
file(GLOB utilsPath src/utils/*)

include_directories(src src/utils)

add_library(utils ${utilsPath})

add_library(chip8disasmLib SHARED ${chip8} ${chip8disasm})
target_compile_definitions(chip8disasmLib PRIVATE CHIP_DISASM)

add_executable(chip8emu ${chip8} ${chip8emu})
target_include_directories(chip8emu PRIVATE ${SDL2_INCLUDE_DIR})
target_link_libraries(chip8emu ${SDL2_LIBRARY} ${OPENGL_gl_LIBRARY} utils chip8disasmLib)
target_compile_definitions(chip8emu PRIVATE CHIP_EMU)
set_target_properties(chip8emu PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

add_custom_command(TARGET chip8emu PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/font.bmp ${CMAKE_BINARY_DIR})
if(WIN32)
add_custom_command(TARGET chip8emu PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/libs/pcre/bin/ ${CMAKE_BINARY_DIR})
add_custom_command(TARGET chip8emu PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/libs/SDL2/bin/x86/ ${CMAKE_BINARY_DIR})
endif()

add_executable(chip8asm ${chip8} ${chip8asm})
target_include_directories(chip8asm PRIVATE ${PCRE_INCLUDE_DIRS})
target_link_libraries(chip8asm ${PCRE_LIBRARY})
target_compile_definitions(chip8asm PRIVATE CHIP_ASM)

add_executable(chip8disasm ${chip8} ${chip8disasm})
target_compile_definitions(chip8disasm PRIVATE CHIP_DISASM CHIP_DISASM_STANDALONE)


