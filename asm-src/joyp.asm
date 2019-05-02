//joyp.asm

.non-default destination

.title
keypad test
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
//select direction keys
ld $hl 0xff00
ld #hl 0x20

//set 8x8 sprites
ld $hl 0xff40
ld $a #hl
//used to clear 3rd bit of [0xff40]
//ld $b 0xfb
//and $b
//used to set 3rd bit of [0xff40]
ld $b 0x04
or $b
ld #hl $a

//initial sprite x coordinate
ld $b 0x10
//increment x coordinate by $c if right button is not pressed
ld $c 0x01
//initial sprite y coordinate
ld $d 0x10
//increment y coordinate by $e if B button is not pressed
ld $e 0x01
jp 0x1000

.0x1000
ld $hl 0xfe00
//set sprite's y coordinate
ld #hl $d
inc $hl
//set sprite's x coordinate
ld #hl $b

//see if right button is pushed down
ld $hl 0xff00
ld #hl 0x20
ld $a 0x01
and #hl
jpnz 0x2000
ld $a 0x00
add $b
add $c
ld $b $a
call 0x4000
jp 0x2000

.0x2000
//see if B button is pushed down
ld $hl 0xff00
ld #hl 0x10
ld $a 0x02
and #hl
jpnz 0x1000
ld $a 0x00
add $d
add $e
ld $d $a
call 0x4000

jp 0x1000

//calling 0x2000 is a very rudimentary wait function
//this works by running nop until the return statement below is executed
//explicitly writing 0x00 to 0x2000 for clarity 
//a better way to implement wait would be to use a register as a counter for a loop
//that way the maximum time delay won't be limited by memory or maximum call stack (if there is one?)
//1000 nops is approx 250us delay
.0x4000
nop
.0x5000
ret

.0x0000
//ld $hl 0xff44
//ld $a #hl
//ld $b 0x91
//cp $b
//jpnz 0x0000
//ld $hl 0xff40
//ld #hl 0x06
//ret
