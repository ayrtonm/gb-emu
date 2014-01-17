#include <stdio.h>
#include "mem.h"
#include "z80.h"
#include "gpu.h"
#include "log.h"
#include "input.h"

void init_abstractions(void)
{
  update_palette(0,obp0);
  update_palette(1,obp1);
  update_palette(2,bgp);
  init_input();
//  init_bank(mbc);
}

int main(int argc, char *argv[])
{
  //write a better help text and a better way to parse arguments
  if (argc < 2) {printf("No rom file given\n");return 0;}

  //remove old log.txt and start a new one
  init_log(LOG_FILE);

  //read rom file and display cartridge info
  cart = load_cart(argv[1]);
//  int mbc = print_header(cart);

  //initialize registers and z80 struct
  z80 *gameboy = init_z80();

  //initialize sdl and pointer to screen
  screen = init_gpu();

  //palette and input variables for now
  //can include any other general abstractions if necessary
  init_abstractions();

  //self explanatory
  emulate(gameboy);
  return 0;
}
