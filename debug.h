#ifndef DEBUG_H
#define DEBUG_H
#include "cpu.h"
#include "common.h"

uint8 convert_hex(char *c);

void print_help(void);
void print_registers(cpu *);
void test_opcode(uint8, uint8, cpu *);
void debug_mode(cpu *);
uint16 handle_args(int argc, char *argv[]);

#endif
