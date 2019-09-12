# gb-emu
![demo](https://github.com/ayrtonm/gb-emu/blob/master/demo.gif)  
This is a gameboy emulator started in 2013 as a personal project to learn about computer architecture. After years of on and off (mostly off) development it now works well enough to run many games though it is still a work in progress. Currently the display and keypad are the only implemented I/O peripherals (no sound yet). Memory bank switching (required for ROMs larger than 32 kB) is implemented for a few common memory bank controllers. The processor currently works by decoding and interpreting opcodes one at a time, but a dynamic recompiler (dynarec) version is in the works. The dynarec uses [LibJIT](https://www.gnu.org/software/libjit/) to compile continuous sequences of gameboy opcodes into executable native code. This version is mainly a way to learn to about just-in-time (JIT) compilation since the interpreted cpu is more than fast enough and many components are shared between the two versions.

## Features
- fully-functional display and keyboard input
- reads an optional configuration file for display and keyboard settings 
- automatically loads and saves external RAM to a file
- save and reload the emulator states at any point
- option to dump contents of addressable memory to a file on exit (for debugging)
- memory bank switching for MBC1, MBC2, MBC3 (partially implemented for MBC5)

## Building
Install SDL2. Run `make`.

## Usage
    Usage: gb-emu [OPTION...] -l FILENAME
      -c, --configfile=CONFIGFILE   load the given configuration file
      -d, --memorydump=OUTPUTFILE   print addresses and values of non-zero memory
                                 into OUTFILE at the end of emulation
      -l, --inputfile=INPUTFILE  load the given file and start the emulator
      -r                         use dynamic recompilation
      -s, --savefile=SAVEFILE    load and save external RAM to the following file
      -?, --help                 Give this help list
          --usage                Give a short usage message

## Default Keybindings

| Key       | Action          | Configuration string   |
| --------- | --------------- | ---------------------- |
| Up        | Up              | key.up                 |
| Down      | Down            | key.down               |
| Left      | Left            | key.left               |
| Right     | Right           | key.right              |
| z         | A               | key.a                  |
| x         | B               | key.b                  |
| Return    | Start           | key.start              |
| Backspace | Select          | key.select             |
| q         | Quit            | key.quit               |
| Space     | Toggle throttle | key.throttle           |
| p         | Save state      | key.save               |
| o         | Load state      | key.load               |
| r         | Reset game      | key.reset              |

## Other Configuration Options

Config files should contain one line per setting with the configuration string and value. Examples are available in [settings.cfg](https://github.com/ayrtonm/gb-emu/blob/master/settings.cfg).

| Setting               | Configuration string | Value type | Default value |
| --------------------- | -------------------- | ---------- | ------------- |
| horizontal resolution | lcd.x                | int        | 160           |
| vertical resolution   | lcd.y                | int        | 144           |
| resizable display     | lcd.resizable        | bool       | true          |
| fullscreen display    | lcd.fullscreen       | bool       | false         |

 
## Testing
**TODO** insert blurb about my half-baked assembler ~~python~~ Go script  
**TODO** insert blurb about [gb-test-roms repo](https://github.com/retrio/gb-test-roms) and which tests are currently passing/failing

## Useful references

- [Pan Docs](https://problemkaputt.de/pandocs.htm)
- [Gameboy CPU (LR35902) instruction set](http://www.pastraiser.com/cpu/gameboy/gameboy_opcodes.html)
- [gb-test-roms](https://github.com/retrio/gb-test-roms)
- [Mednafen](https://mednafen.github.io/)
- [Dynarec Guide](https://github.com/marco9999/Dynarec_Guide)
- [jamulator](https://andrewkelley.me/post/jamulator.html)
- [Corrosion](https://bheisler.github.io/post/experiments-in-nes-jit-compilation/)

