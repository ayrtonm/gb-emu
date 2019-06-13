#gb-emu
------
This is a gameboy emulator started in 2013 as a personal project to learn about computer architecture. After years of on and off (mostly off) development it now works well enough to run real games though it is still a work in progress. Currently the display and keypad are the only implemented I/O peripherals. Memory bank switching (required for ROMs largers than 32 kB) is implemented for a few common memory bank controllers. The processor is currently emulated by an interpreter that decodes opcodes one at a time. This works well, but I'm also working on a dynamic recompilation option using LLVM to learn about just-in-time (JIT) compilation.

##Features
- (mostly working) display
- keypad input
- memory bank switching for MBC1, MBC2, MBC3
- internal timers

##Building
**TODO** actually write something useful here

`make`

only tested with g++ and clang++

##Usage

    Usage: gb-emu [OPTION...] -l FILENAME
      -d, --memorydump=OUTFILE   print addresses and values of non-zero memory into
                                 OUTFILE at the end of emulation
      -l, --inputfile=FILENAME   load the given file and start the emulator
      -r                         use dynamic recompilation
      -?, --help                 Give this help list
          --usage                Give a short usage message
 
##Testing
**TODO** insert blurb about the half-baked python assembler

**TODO** insert blurb about test roms

##More info and acknowledgments
**TODO** fixed the text in the following links

- [pandocs](http://bgb.bircd.org/pandocs.htm)
- [big list of opcodes](http://www.pastraiser.com/cpu/gameboy/gameboy_opcodes.html)
- [mednafen](https://mednafen.github.io/)
- [test roms repo](https://github.com/retrio/gb-test-roms)
- [statically recompiled NES emulator](https://andrewkelley.me/post/jamulator.html)
- [dynamically recompiled NES emulator](https://bheisler.github.io/post/experiments-in-nes-jit-compilation/)
- [dynamic recompiler guide](https://github.com/marco9999/Dynarec_Guide)
