#include <stdio.h>
#include <string.h>
#include "debug.h"

void print_help(void)
{
  printf("Gameboy Emulator\n");
  printf("Usage:\n");
  printf("  -h              print this help text\n");
  printf("  -t   [opcode]   test the given opcode\n");
  printf("  -cb  [opcode]   test the given 0xCB opcode\n");
  printf("  -r   [filename] load the given file and start the emulator\n");
  printf("  -i              interactive debug mode\n");
}

uint16 handle_args(int argc, char *argv[])
{
  if (argc < 2) {return 0;}
  int i;
  for (i = 1; i < argc; i++)
  {
    if (!strcmp(argv[i],"-h")) {return 0;}
/*    else if (!strcmp(argv[i],"-i")) {return 2;}
    else if (argc < 3) {return 0;}
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
