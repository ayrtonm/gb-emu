#include "globals.h"
#include "cpu.h"//initialize cpu struct
#include "mem.h"//load rom and initialize memory struct
#include "gpu.h"//initialize screen surface
#include "debug.h"//handling arguments no actual debugging yet
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  uint16 action = handle_args(argc,argv);
  if (action == 0) {print_help();}
  else if (action == 1)
  {
    gameboy = malloc(sizeof(gb));
    gameboy->cpu = init_cpu();
    gameboy->gpu = init_gpu();
    uint8 *cart = load_cart(argv[2]);
    gameboy->mbc = parse_header(cart);
    gameboy->mem = init_mem();
    emulate();
  }
  return 0;
}