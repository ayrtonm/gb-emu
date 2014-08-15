#include "globals.h"
#include "cpu.h"//initialize cpu struct
#include "mem.h"//load rom and initialize memory struct
#include "lcd.h"//initialize screen surface

void print_help(void)
{
  printf("Gameboy Emulator\n");
  printf("Usage:\n");
  printf("  -h              print this help text\n");
  printf("  -r   [filename] load the given file and start the emulator\n");
  printf("  -p   [filename] load file and print executed opcodes to stdout\n");
  printf("  -m   [filename] load file and print memory writes to stdout\n");
  printf("  -d   [filename] load file in debug mode\n");
}

uint16 handle_args(int argc, char *argv[])
{
  int i;
  for (i = 1; i < argc; i++)
  {
    if (!strcmp(argv[i],"-h") || argc < 3) {return 0;}
    else if (!strcmp(argv[i],"-r")) {printing = run;return 1;}
    else if (!strcmp(argv[i],"-p")) {printing = opcodes;return 1;}
    else if (!strcmp(argv[i],"-d")) {printing = debug;return 1;}
    else if (!strcmp(argv[i],"-m")) {printing = memory;return 1;}
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
    gameboy->key_bitmap = 0;
    emulate();
  }
  return 0;
}
