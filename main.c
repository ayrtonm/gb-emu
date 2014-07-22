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
  printf("  -r   [filename] load the given file and start the emulator\n");
  printf("  -rp  [filename] load file and print executed opcodes to stdout\n");
  printf("  -rd  [filename] load file in debug mode\n");
}

uint16 handle_args(int argc, char *argv[])
{
  if (argc < 2) {return 0;}
  int i;
  for (i = 1; i < argc; i++)
  {
    if (!strcmp(argv[i],"-h")) {return 0;}
    //remove next line if adding opcode test and interactive debug mode
    //07/20/2014 NEVER ADD INTERACTIVE DEBUG MODE EVER AGAIN
    //IT ONLY ADDS UNNECESSARY COMPLEXITY AND SHIT
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
    else if (!strcmp(argv[i],"-r")) {printing = 0;return 1;}
    else if (!strcmp(argv[i],"-rp")) {printing = 1;return 1;}
    else if (!strcmp(argv[i],"-rd")) {printing = 2;return 1;}
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
    gameboy->time_period = 0;
    gameboy->time_clk = 0;
    gameboy->div_clk = 0;
    gameboy->joyp[0] = 0xFF;
    gameboy->joyp[1] = 0xFF;
    emulate();
  }
  return 0;
}