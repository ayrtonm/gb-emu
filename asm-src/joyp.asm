//joyp.asm
//should've been joypad test
//turned out to be almost everything beside joypad

.non-default destination

.title
scroll right
//0x435553544f4d20524f4d

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
//counter for x-coordinate of sprite
ld $a 0x00
//increment for x-coordinate of sprite
ld $b 0x01

.0x1000
//place sprite
ld $hl 0xfe00
//set y-coordinate
ld #hl 0x10
//move to address that controls x-coordinate
inc $hl
//push $hl

//set vblank before updating sprite position to prevent tearing
//ld $hl 0xff40
//ld #hl 0x84
//pop $hl

ld #hl $a

//ld $hl 0xff40
//ld #hl 0x86

add $b
call 0x2000
jp 0x1000

//calling 0x2000 is a very rudimentary wait function
//this works by running nop until the return statement below is executed
//explicitly writing 0x00 to 0x2000 for clarity 
//a better way to implement wait would be to use a register as a counter for a loop
//that way the maximum time delay won't be limited by memory or maximum call stack (if there is one?)
//1000 nops is approx 250us delay
.0x2000
nop
.0x6000
ret

.0x0000
ld $hl 0xff44
ld $a #hl
ld $b 0x91
cp $b
jpnz 0x0000
ld $hl 0xff40
ld #hl 0x06
ret
