.title
conditional test

//0x0100
start:
  jp main

//0x0150
main:
  ld $b 5
  //0x0152
  loop:
    dec $b
    //0x0153
    jpnz loop
  jp end

//0x0159
end:
  jp end
