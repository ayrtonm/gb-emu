title:
test rom

start:
jp 0x0150

0x0150:
ld $b, $b
ld $b, $a
rst 0x08
