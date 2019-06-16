# gb-emu
------
This is a gameboy emulator started in 2013 as a personal project to learn about computer architecture. After years of on and off (mostly off) development it now works well enough to run some games though it is still a work in progress. Currently the display and keypad are the only implemented I/O peripherals (no sound yet). Memory bank switching (required for ROMs larger than 32 kB) is implemented for a few common memory bank controllers. The processor is currently emulated like an interpreter that decodes opcodes one at a time. This works well, but I'm also working on a dynamic recompilation option using LLVM to learn about just-in-time (JIT) compilation.

## Features
- video display
- keypad input
- memory bank switching for MBC1, MBC2, MBC3
- internal timer and real-time clock registers
- interrupts
- option to dump contents of addressable memory to a file on exit (for debugging)

## Building
Install SDL2. Run `make`.

## Usage
    Usage: gb-emu [OPTION...] -l FILENAME
      -d, --memorydump=OUTFILE   print addresses and values of non-zero memory into
                                 OUTFILE at the end of emulation
      -l, --inputfile=FILENAME   load the given file and start the emulator
      -?, --help                 Give this help list
          --usage                Give a short usage message
 
## Testing
**TODO** insert blurb about my half-baked assembler python script

**TODO** insert blurb about gb-test-roms repo and which tests are currently passing/failing

## Useful references

- [Pan Docs](https://problemkaputt.de/pandocs.htm)
- [Gameboy CPU (LR35902) instruction set](http://www.pastraiser.com/cpu/gameboy/gameboy_opcodes.html)
- [gb-test-roms](https://github.com/retrio/gb-test-roms)
- [Mednafen](https://mednafen.github.io/)
- [Dynarec Guide](https://github.com/marco9999/Dynarec_Guide)
- [jamulator](https://andrewkelley.me/post/jamulator.html)
- [Corrosion](https://bheisler.github.io/post/experiments-in-nes-jit-compilation/)

