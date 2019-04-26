//oam.asm
//testing oam stuff

.non-default destination

.title
0x435553544f4d20524f4d

.start
jp 0x0150

.0x0150
//load sprite tile data
ld $hl 0x8002
ld #hl 0x3c
inc $hl
inc $hl
ld #hl 0x66
inc $hl
ld #hl 0x18
inc $hl
ld #hl 0x4a
inc $hl
ld #hl 0x3c
inc $hl
ld #hl 0x42
inc $hl
ld #hl 0x3c
inc $hl
ld #hl 0x66
inc $hl
ld #hl 0x18
inc $hl
ld #hl 0x3c
//load sprite tile data
ld $hl 0x8012
ld #hl 0x3c
inc $hl
inc $hl
ld #hl 0x66
inc $hl
ld #hl 0x18
inc $hl
ld #hl 0x4a
inc $hl
ld #hl 0x3c
inc $hl
ld #hl 0x42
inc $hl
ld #hl 0x3c
inc $hl
ld #hl 0x66
inc $hl
ld #hl 0x18
inc $hl
ld #hl 0x3c

//load palette (obp0)
ld $hl 0xff48
ld #hl 0x1b

//disable bkg, win/enable sprites
ld $hl 0xff40
ld #hl 0x86

ld $hl 0xfe00
ld #hl 0x10
inc $hl
ld #hl 0x08

ld $hl 0xfe04
ld #hl 0x28
inc $hl
ld #hl 0x20

ld $hl 0xfe08
ld #hl 0x38
inc $hl
ld #hl 0x30
jp 0x0200

.0x0200
nop
jp 0x0200
