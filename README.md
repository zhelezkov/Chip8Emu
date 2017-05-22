# Chip8Emu
CHIP-8 is an assembler, disassembler, debugger, and emulator for the [COSMAC ELF](https://en.wikipedia.org/wiki/RCA_1802) [CHIP-8](https://en.wikipedia.org/wiki/CHIP-8) interpreter. 

Chip-8Emu is written in C++ and uses [SDL](https://www.libsdl.org/) for its rendering, input handling, and audio. It should easily run on Windows and OSX

## Building
**You must have the latest version [cmake](https://cmake.org/) and have [GCC](https://gcc.gnu.org/) compile for the OSX or have [Visual Studio 2017](https://www.visualstudio.com/ru/downloads/) for the Windows**
* Clone or [download](https://github.com/rsredsq/Chip8Emu/archive/master.zip) source code
* Run the script file depending on your operating system. buildWIN.bat for Windows and buildOSX.sh for OSX
* Have fun!

# What's a Chip-8?
Chip-8 is a simple, interpreted, programming language which was first used on some do-it-yourself computer systems in the late 1970s and early 1980s. The COSMAC VIP, DREAM 6800, and ETI 660 computers are a few examples. These computers typically were designed to use a television as a display, had between 1 and 4K of RAM, and used a 16-key hexadecimal keypad for input. The interpreter took up only 512 bytes of memory, and programs, which were entered into the computer in hexadecimal, were even smaller. CHIP-8 was used to create games such as Pong, Tetris, Space Invaders, etc.

In 1990 Andreas Gustafsson wrote interpreter CHIP-8 for the graphing calculator HP-48. This version was called CHIP-48.

In 1991 Erik Bryntse added to the language several innovations that have resulted in a 2-fold increase in screen resolution in the games and use scrolling. This version was called Super Chip-8.

Currently, there are more and MegaChip developed by Revival Studios. In this version added color graphics and sound.

# Screen
