.title
  jit test

start:
  jp main

main:
  ld $bc 0x1000
  //this pushes the address of f onto the stack
  call f
  call g
  jp main

.data
  0xffffffff
  0xffffffff

//don't pop before pushing to not mess up the call stack
//0x0164
f:
  push $bc
  pop $bc
  ret

.data
  0xffffffff
  0xffffffff

//0x016f
g:
  push $bc
  pop $bc
  ret
