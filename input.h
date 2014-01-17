#ifndef INPUT_H
#define INPUT_H
#include "mem.h"

//values for rows shouldn't exceed 0x0F
uint8 rows[2];

//column = 00XY 0000
uint8 column;

extern void init_input(void);
extern int check_input(void);

#endif