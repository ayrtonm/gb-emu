.title
  jit test

start:
  jp main

main:
  ld $bc 0x1000
  //this pushes the address of f onto the stack
  call f
  call g
  call h
  call k
  call j
  jp main

//don't pop before pushing to not mess up the call stack
f:
  push $bc
  pop $bc
  ret

g:
  push $bc
  pop $bc
  ret

h:
  push $bc
  pop $bc
  ret

k:
  push $bc
  pop $bc
  ret

j:
  push $bc
  pop $bc
  ret
