#ifndef MEM_H
#define MEM_H
#include "globals.h"

extern uint8 *load_cart(char *filename);
extern mbc parse_header(uint8 *cart);
extern mem init_mem(void);
extern void write_byte(uint16,uint8);
extern void write_word(uint16,uint16);
extern void write_cart(uint16,uint8);

#endif