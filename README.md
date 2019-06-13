Gb-emu is a Game Boy/Game Boy Color Emulator for Linux. This project was started in June 2013 but is still a work in progress. Gb-emu was started as an educational project so I could learn more about general computer architecture and the underlying operations of the old game console. Since gb-emu is currently a work in progress, most of the command line arguments are used for debugging the emulator by printing out information and aren't really useful for running games.  I am considering adding a completely revamped debug UI (similar to RealBoy's UI) after everything is working. Run with the -h flag to see all options. In no particular order special thanks goes to the following projects: 'RealBoy', 'Mednafen', 'VisualBoyAdvance'

File List
---------
Makefile
README
assembler.py - part of a different project that was used for debugging. doesn't have all opcodes implemented yet, but produces functional code. example source code in file as comments.
cb_opcodes.h - cb opcodes switch case statement
cpu.cpp - initialize cpu and emulate functions
cpu.h - opcode macros and constant tables
config - doxygen config file for documentation
diffscript.sh - script to run specified ROM with gb-emu and modified mednafen. prints out opcodes, removes header text and prints out difference between opcodes.
bits.h - bit manipulation, uint8/uint16 structs and typedefs, macros for memory addresses and constants
incorrect_opcodes - text from debugging. note: gb-emu only prints out opcodes or memory accesses by default. most of the text file was written by me
lcd.cpp - initialize lcd, update palettes, step lcd and drawing functions
lcd.h - lcd class
main.cpp - handles arguments and runs initialize functions
mem.cpp - load cart, parse cart header, initialize memory, and write to memory
mem.h - mem and color classes
opcodes.h - regular opcode switch case statement. only 0xf8 ld hl, sp+r8 isn't implemented, but it's not used by any roms I've tried and should be easy to write
