.title
  jit test

start:
  jp main

main:
  ld $bc 0x1000
  call push
  call pop
  jp main

pop:
  pop $bc
  ret

push:
  push $bc
  ret
