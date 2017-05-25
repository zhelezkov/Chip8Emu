[![alt text](car.gif)](https://rsredsq.github.io/Chip8Emu/)

# Chip8Emu
CHIP-8 is an assembler, disassembler, debugger, and emulator for the [COSMAC ELF](https://en.wikipedia.org/wiki/RCA_1802) [CHIP-8](https://en.wikipedia.org/wiki/CHIP-8) interpreter. 

Chip-8Emu is written in C++, uses [SDL](https://www.libsdl.org/) for its rendering, input handling, and audio and uses [pcre](http://www.pcre.org/) for parsing assembly. It should easily run on Windows and OSX.

Athors: [rsredsq](https://github.com/rsredsq) and [scrat98](https://github.com/scrat98)

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

## Memory
CHIP-8 has 4 KB of memory (addresses 0x000h-0xFFFh). The first 512 bytes (addresses 0x000h-0x200h) are reserved for the interpreter, so the game is available only 3,584 bytes. Accordingly, the game is in memory starting at address 0x200h.

```
+---------------+= 0xFFF (4095) End of Chip-8 RAM
|               |
|               |
|               |
|               |
|               |
| 0x200 to 0xFFF|
|     Chip-8    |
| Program / Data|
|     Space     |
|               |
|               |
|               |
|               |
|               |
|               |
|               |
+---------------+= 0x200 (512) Start of most Chip-8 programs
| 0x000 to 0x1FF|
| Reserved for  |
|  interpreter  |
+---------------+= 0x000 (0) Start of Chip-8 RAM
```

## Screen
The original implementation of the CHIP-8 has a monochrome(black and white) screen size 64х32 pixel. Super Chip in addition to the main advanced mode is 128x64. 

Chip-8 draws graphics on screen through the use of sprites. A sprite is a group of bytes which are a binary representation of the desired picture. Chip-8 sprites may be up to 15 bytes, for a possible sprite size of 8x15. The interpreter provides 16 predefined sprites with a size of 4x5 pixels(8x10 for the Super Chip). It is a hexadecimal number from 0 to F.

## Keyboard
Computers which used the original CHIP-8 had 16-key keyboard. for convenience, all the keys were located to the left

```
Original              In the Emulator
+-+-+-+-+                +-+-+-+-+
|1|2|3|C|                |1|2|3|4|
+-+-+-+-+                +-+-+-+-+
|4|5|6|D|                |Q|W|E|R|
+-+-+-+-+       =>       +-+-+-+-+
|7|8|9|E|                |A|S|D|F|
+-+-+-+-+                +-+-+-+-+
|A|0|B|F|                |Z|X|C|V|
+-+-+-+-+                +-+-+-+-+
```
## Registers
The CHIP-8 has 16, 8-bit virtual registers: `V0`, `V1`, `V2`, `V3`, `V4`, `V5`, `V6`, `V7`, `V8`, `V9`, `VA`, `VB`, `VC`, `VD`, `VE`, and `VF`. All of these are considered general purpose registers except for `VF` which is used for carry, borrow, shift, overflow, and collision detection.
 
There is a single, 16-bit address register: `I`, which is used for reading from - and writing to - memory.

Last, there are two, 8-bit, timer registers (`DT` for delays and `ST` for sounds) that continuously count down at 60 Hz. The delay timer is good for time limiting your game or waiting brief periods of time. While the sound timer is non-zero a tone will be emitted.

Finally, the Super CHIP-8, which was used on the HP-48 calculators, contained 8, 8-bit, user-flag registers: `R0`-`R7`. These cannot be directly used, but registers `V0`-`V7` can be saved to - and loaded from - them. This can be quite handy at times. See the `LD R, VX` and `LD VX, R` instructions below.

## Stack & CPU
In the stack are saved the return address in function calls. The original interpreter had 12 levels of nesting stack. Now we are decided to do 16. Each value in the stack has a size of 2 bytes.

The processor executes 1000 opcodes per second

## Timers & Sound
Chip-8 provides 2 timers, a delay timer and a sound timer.

The delay timer is active whenever the delay timer register (DT) is non-zero. This timer does nothing more than subtract 1 from the value of DT at a rate of 60Hz. When DT reaches 0, it deactivates.

The sound timer is active whenever the sound timer register (ST) is non-zero. This timer also decrements at a rate of 60Hz, however, as long as ST's value is greater than zero, the Chip-8 buzzer will sound. When ST reaches zero, the sound timer deactivates.

## Instructions
<table border=1 frame=void rules=all>
<thead>
<tr>
<th align="left">Opcode</th>
<th align="left">Mnemonic</th>
<th align="left">Description</th>
</tr>
</thead>
<tbody>
<tr>
<td align="left">00E0</td>
<td align="left">CLS</td>
<td align="left">Clear video memory</td>
</tr>
<tr>
<td align="left">00EE</td>
<td align="left">RET</td>
<td align="left">Return from subroutine</td>
</tr>
<tr>
<td align="left">0NNN</td>
<td align="left">SYS</td>
<td align="left">NNN	Call CDP1802 subroutine at NNN</td>
</tr>
<tr>
<td align="left">00BN</td>
<td align="left">SCU</td>
<td align="left">N	Scroll up N pixels (N/2 pixels in low res mode)</td>
</tr>
<tr>
<td align="left">00CN</td>
<td align="left">SCD</td>
<td align="left">N	Scroll down N pixels (N/2 pixels in low res mode)</td>
</tr>
<tr>
<td align="left">00FB</td>
<td align="left">SCR</td>
<td align="left">Scroll right 4 pixels (2 pixels in low res mode)</td>
</tr>
<tr>
<td align="left">00FC</td>
<td align="left">SCL</td>
<td align="left">Scroll left 4 pixels (2 pixels in low res mode)</td>
</tr>
<tr>
<td align="left">00FD</td>
<td align="left">EXIT</td>
<td align="left">Exit the interpreter; this causes the VM to infinite loop</td>
</tr>
<tr>
<td align="left">00FE</td>
<td align="left">LOW</td>
<td align="left">Enter low resolution (64x32) mode; this is the default mode</td>
</tr>
<tr>
<td align="left">00FF</td>
<td align="left">HIGH</td>
<td align="left">Enter high resolution (128x64) mode</td>
</tr>
<tr>
<td align="left">1NNN</td>
<td align="left">JP</td>
<td align="left">NNN	Jump to address NNN</td>
</tr>
<tr>
<td align="left">2NNN</td>
<td align="left">CALL</td>
<td align="left">NNN	Call CHIP-8 subroutine at NNN</td>
</tr>
<tr>
<td align="left">3XNN</td>
<td align="left">SE</td>
<td align="left">VX, NN	Skip next instruction if VX == NN</td>
</tr>
<tr>
<td align="left">4XNN</td>
<td align="left">SNE</td>
<td align="left">VX, NN	Skip next instruction if VX != NN</td>
</tr>
<tr>
<td align="left">5XY0</td>
<td align="left">SE</td>
<td align="left">VX, VY	Skip next instruction if VX == VY</td>
</tr>
<tr>
<td align="left">5XY1</td>
<td align="left">SGT</td>
<td align="left">VX, VY	Skip next instruction if VX &gt; VY</td>
</tr>
<tr>
<td align="left">5XY2</td>
<td align="left">SLT</td>
<td align="left">VX, VY	Skip next instruction if VX &lt; VY</td>
</tr>
<tr>
<td align="left">6XNN</td>
<td align="left">LD VX, NN</td>
<td align="left">VX = NN</td>
</tr>
<tr>
<td align="left">7XNN</td>
<td align="left">ADD VX, NN</td>
<td align="left">VX = VX + NN</td>
</tr>
<tr>
<td align="left">8XY0</td>
<td align="left">LD VX, VY</td>
<td align="left">VX = VY</td>
</tr>
<tr>
<td align="left">8XY1</td>
<td align="left">OR VX, VY</td>
<td align="left">VX = VX OR VY</td>
</tr>
<tr>
<td align="left">8XY2</td>
<td align="left">AND VX, VY</td>
<td align="left">VX = VX AND VY</td>
</tr>
<tr>
<td align="left">8XY3</td>
<td align="left">XOR VX, VY</td>
<td align="left">VX = VX XOR VY</td>
</tr>
<tr>
<td align="left">8XY4</td>
<td align="left">ADD VX, VY</td>
<td align="left">VX = VX + VY; VF = 1 if overflow else 0</td>
</tr>
<tr>
<td align="left">8XY5</td>
<td align="left">SUB VX, VY</td>
<td align="left">VX = VX - VY; VF = 1 if not borrow else 0</td>
</tr>
<tr>
<td align="left">8XY6</td>
<td align="left">SHR VX</td>
<td align="left">VF = LSB(VX); VX = VX » 1</td>
</tr>
<tr>
<td align="left">8XY7</td>
<td align="left">SUBN VX, VY</td>
<td align="left">VX = VY - VX; VF = 1 if not borrow else 0</td>
</tr>
<tr>
<td align="left">8XYE</td>
<td align="left">SHL VX</td>
<td align="left">VF = MSB(VX); VX = VX « 1</td>
</tr>
<tr>
<td align="left">9XY0</td>
<td align="left">SNE VX, VY</td>
<td align="left">Skip next instruction if VX != VY</td>
</tr>
<tr>
<td align="left">ANNN</td>
<td align="left">LD I, NNN</td>
<td align="left">I = NNN</td>
</tr>
<tr>
<td align="left">BNNN</td>
<td align="left">JP V0, NNN</td>
<td align="left">Jump to address NNN + V0</td>
</tr>
<tr>
<td align="left">CXNN</td>
<td align="left">RND VX, NN</td>
<td align="left">VX = RND() AND NN</td>
</tr>
<tr>
<td align="left">DXYN</td>
<td align="left">DRW VX, VY, N</td>
<td align="left">Draw 8xN sprite at I to VX, VY; VF = 1 if collision else 0</td>
</tr>
<tr>
<td align="left">DXY0</td>
<td align="left">DRW VX, VY, 0</td>
<td align="left">Draw a 16x16 sprite at I to VX, VY (8x16 in low res mode)</td>
</tr>
<tr>
<td align="left">EX9E</td>
<td align="left">SKP VX</td>
<td align="left">Skip next instruction if key(VX) is pressed</td>
</tr>
<tr>
<td align="left">EXA1</td>
<td align="left">SKNP VX</td>
<td align="left">Skip next instruction if key(VX) is not pressed</td>
</tr>
<tr>
<td align="left">FX07</td>
<td align="left">LD VX, DT</td>
<td align="left">VX = DT</td>
</tr>
<tr>
<td align="left">FX0A</td>
<td align="left">LD VX, K</td>
<td align="left">Wait for key press, store key pressed in VX</td>
</tr>
<tr>
<td align="left">FX15</td>
<td align="left">LD DT, VX</td>
<td align="left">DT = VX</td>
</tr>
<tr>
<td align="left">FX18</td>
<td align="left">LD ST, VX</td>
<td align="left">ST = VX</td>
</tr>
<tr>
<td align="left">FX1E</td>
<td align="left">ADD I, VX</td>
<td align="left">I = I + VX; VF = 1 if I &gt; 0xFFF else 0</td>
</tr>
<tr>
<td align="left">FX29</td>
<td align="left">LD F, VX</td>
<td align="left">I = address of 4x5 font character in VX (0..F)</td>
</tr>
<tr>
<td align="left">FX33</td>
<td align="left">LD B, VX</td>
<td align="left">Store BCD representation of VX at I (100), I+1 (10), and I+2 (1); I remains unchanged</td>
</tr>
<tr>
<td align="left">FX55</td>
<td align="left">LD [I], VX</td>
<td align="left">Store V0..VX (inclusive) to memory starting at I; I remains unchanged</td>
</tr>
<tr>
<td align="left">FX65</td>
<td align="left">LD VX, [I]</td>
<td align="left">Load V0..VX (inclusive) from memory starting at I; I remains unchanged</td>
</tr>
<tr>
<td align="left">FX30</td>
<td align="left">LD HF, VX</td>
<td align="left">I = address of 8x10 font character in VX (0..F)</td>
</tr>
<tr>
<td align="left">FX75</td>
<td align="left">LD R, VX</td>
<td align="left">Store V0..VX (inclusive) into HP-RPL user flags R0..RX (X &lt; 8)</td>
</tr>
<tr>
<td align="left">FX85</td>
<td align="left">LD VX, R</td>
<td align="left">Load V0..VX (inclusive) from HP-RPL user flags R0..RX (X &lt; 8)</td>
</tr>
</tbody>
</table>

# Emulator
<table border=1 frame=void rules=all>
<thead>
<tr>
<th align="left">Comamnd</th>
<th align="left">Description</th>
</tr>
</thead>
<tbody>
<tr>
<td align="left"><code>F1</code></td>
<td align="left">Load ROM</td>
</tr>
<tr>
<td align="left"><code>Ctr + R</code></td>
<td align="left">Restart ROM</td>
</tr>
<tr>
<td align="left"><code>Ctr + G</code></td>
<td align="left">Open debug window</td>
</tr>
</tbody>
</table>

# Debugger
<table border=1 frame=void rules=all>
<thead>
<tr>
  <th align="left">Comamnd</th>
  <th align="left">Description</th>
</tr>
</thead>
<tbody>
<tr>
  <td align="left"><code>Space</code></td>
  <td align="left">Pause</td>
</tr>
<tr>
  <td align="left"><code>N</code></td>
  <td align="left">Next instruction</td>
</tr>
</tbody>
</table>

# Assembler
If you want to try creating your own game, then this is what you need. The sample programs can be viewed [here](https://github.com/rsredsq/Chip8Emu/tree/master/games/tests). This is a wonderful interpreter, who will point out your mistakes, making it easier to create games

## Syntax
<table border=1 frame=void rules=all>
  <tbody>
  <tr>
    <th align="left">Command</th>
    <th align="left">Syntax</th>
    <th align="left">Example</th>
  </tr>
  <tr>
    <td align="left"><code>Labels</code></td>
    <td align="left">label:</td>
    <td align="left">start:</td>
  </tr>
  <tr>
    <td rowspan="3" align="left"><code>Bytes</code></td>
    <td rowspan="3" align="left">byte</td>
    <td align="left"><code>hex</code> byte #A3</td>
  </tr>
  <tr>
    <td align="left"><code>bin</code> byte %10010011</td>
  </tr>
  <tr>
    <td align="left"><code>dec</code> byte 125</td>
  </tr>
  <tr>
    <td align="left"><code>Define</code></td>
    <td align="left">equ [name] [value]</td>
    <td align="left">equ up 5</td>
  </tr>
  <tr>
    <td align="left"><code>Variable</code></td>
    <td align="left">var [name] [register]</td>
    <td align="left">var speed v3</td>
  </tr>
  <tr>
    <td align="left"><code>Comments</code></td>
    <td align="left">;</td>
    <td align="left">; this is comment</td>
  </tr>  
</tbody>
</table>

## Code generate
Using the command line, run chip8asm.exe by the assembler file name as a parameter. As a result, you will get a bin file, and two log file.

`Assembler.txt` - it's a parsed file for greater clarity (was done for a static code analyzer, but it was not completed yet)
`Assembler.log` - it's a log with all errors, variables, labels, and other information

# DisAssembler
Using the command line, run chip8disasm.exe by the bin file name as a parameter. As a result, you will get a assembler file.

# Thanks for watching
If you have any questions or find a bug, please contact us: [rsredsq](https://github.com/rsredsq) or [scrat98](https://github.com/scrat98). 
