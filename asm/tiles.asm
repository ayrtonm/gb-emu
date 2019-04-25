//tiles.asm
//example for syntax
//lines starting with '//' are ignored
//hexdecimal strings are written two numbers at a time
//numbers not starting in 0x are decimal(currently ignored?)
//$register directly accesses the register
//#register uses the register as a pointer to memory
//new sections start with '.'
//valid sections are any valid memory address, start, title, and non-japanese for destination code
//assembler probably will add .stop internally to make things easier to keep track of
//default destination code is japan
//this test file copies a specific tile pattern over the screen and scrolls horizontally
.non-default destination

.title
0x435553544f4d20524f4d

.start
//pc initialized to 100
ld $hl 0x8000
ld $de 0x423c
rst 0x00
ld $de 0x00ff
rst 0x00
rst 0x08
rst 0x08
rst 0x08
rst 0x08
ld $de 0x00ff
rst 0x00
ld $de 0x423c
rst 0x00
jp 0x0150

.0x0000
ld #hl $d
inc $l
ld #hl $e
inc $l
ret

.0x0008
ld $de 0x4281
rst 0x00
ret

.0x0150
ld $hl 0xff43
ld $b #hl
inc $b
ld #hl $b
jp 0x0150
