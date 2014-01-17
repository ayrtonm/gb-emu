#ifndef MEM_H
#define MEM_H
#include "common.h"

typedef struct mbc
{
  int version;
  uint8 rombank,rambank,mode,enable;
  uint8 *cart;
} mbc;

static uint16 rom_sizes[8] = {0x0080, 0x0100, 0x0200, 0x0400, 0x0800, 0x1000, 0x2000, 0x4000};
static uint8 ram_sizes[4] = {0x00,0x08,0x20,0x80};

extern uint8 *load_cart(char *filename);
extern mbc *parse_header(char *cart);
extern void write_cart(uint16,uint8);

extern uint8 read_byte(uint16);
extern uint16 read_word(uint16);
extern void write_byte(uint16,uint8);
extern void write_word(uint16,uint16);

#endif
