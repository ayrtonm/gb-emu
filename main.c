#include "cpu.h"
#include "mem.h"
#include "debug.h"
#include "globals.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  uint16 action = handle_args(argc,argv);
  gameboy = malloc(sizeof(gb));
  gameboy->cpu = init_cpu();
  if (action == 0) {print_help();}
  else if (action == 1) //normal rom loading procedure goes here
  {
    uint8 *cartridge = load_cart(argv[2]);
    gameboy->mbc = parse_header(cartridge);
    emulate(gameboy);
  }
/*
  else if (action == 2)
  {
    debug_mode(gameboy);
  }
  else if ((action & 0xFF00) == 0x0100 || (action & 0xFF00) == 0xCB00) //test opcode
  {
    uint8 opcode = action & 0x00FF;
    uint8 type = action >> 8;
    test_opcode(opcode,type,gameboy);
  }
*/
  return 0;
}
