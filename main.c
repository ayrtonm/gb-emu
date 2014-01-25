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
  printf("To use debug mode, compile with #define DEBUG.\n");
  printf("Debug Mode:\n");
  printf("   n   next opcode\n");
  printf("   l   run until LY changes (end of HBLANK or 1/10th of VBLANK)\n");
  printf("   f   run until LY == 155 (end of VBLANK)\n");
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
    gameboy->time_period = 0;
    gameboy->time_clk = 0;
    gameboy->div_clk = 0;
    emulate();
  }
  return 0;
}