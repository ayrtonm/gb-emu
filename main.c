#include "globals.h"
#include "cpu.h"//initialize cpu struct
#include "mem.h"//load rom and initialize memory struct
#include "lcd.h"//initialize screen surface
#include <stdio.h>
#include <stdlib.h>

void print_help(void)
{
  printf("Gameboy Emulator\n");
  printf("Usage:\n");
  printf("  -h              print this help text\n");
  printf("  -t   [opcode]   test the given opcode\n");
  printf("  -cb  [opcode]   test the given 0xCB opcode\n");
  printf("  -r   [filename] load the given file and start the emulator\n");
  printf("  -i              interactive debug mode\n");
  printf("note: test opcode mode is currently unsupported\n");
}

uint16 handle_args(int argc, char *argv[])
{
  if (argc < 2) {return 0;}
  int i;
  for (i = 1; i < argc; i++)
  {
    if (!strcmp(argv[i],"-h")) {return 0;}
    //remove next line if adding opcode test and interactive debug mode
    else if (argc < 3) {return 0;}
    else if (!strcmp(argv[i],"-i")) {return 2;}
/*    else if (argc < 3) {return 0;}
    else if (!strcmp(argv[i],"-t"))
    {
      uint8 opcode = convert_hex(argv[i+1]);
      uint16 ret = (opcode|0x0100);
      return ret;
    }
    else if (!strcmp(argv[i],"-cb"))
    {
      uint8 opcode = convert_hex(argv[i+1]);
      uint16 ret = (opcode|0xCB00);
      return ret;
    }
*/
    else if (!strcmp(argv[i],"-r")) {return 1;}
  }
}

int main(int argc, char *argv[])
{
  uint16 action = handle_args(argc,argv);
  if (action == 0) {print_help();}
  else if (action == 1)
  {
    gameboy = malloc(sizeof(gb));
    gameboy->cpu = init_cpu();
    gameboy->lcd = init_lcd();
    uint8 *cart = load_cart(argv[2]);
    gameboy->mbc = parse_header(cart);
    gameboy->mem = init_mem();
    gameboy->time_period = 
    emulate();
  }
  return 0;
}