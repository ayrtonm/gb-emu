#ifndef MEM_H
#define MEM_H
#include "globals.h"

//initialize mbc struct pointer
extern uint8 *load_cart(char *filename);
extern mbc parse_header(uint8 *cart);

//initialize mem struct pointer
extern mem init_mem(void);

//write word only writes two bytes consecutively
//if writing to certain sections or ROM or RAM, write cart
//is called by write byte
extern void write_cart(uint16,uint8);
extern void write_byte(uint16,uint8);
extern void write_word(uint16,uint16);

//shift 8 to left to use
static uint16 rom_sizes[8] = {0x0080, 0x0100, 0x0200, 0x0400, 0x0800, 0x1000, 0x2000, 0x4000};
static uint8 ram_sizes[4] = {0x00,0x08,0x20,0x80};

#endif