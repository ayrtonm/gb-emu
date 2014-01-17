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
void test_opcode(uint8 opcode, uint8 cb, cpu *Z)
{
  print_registers(Z);
  if (cb == 0xCB)
  {
    _PC += 2;
    switch(opcode)
    {
      #include "cb_opcodes.h"
    }
  }
  else if (cb == 0x01)
  {
    _PC += length[opcode];
    switch(opcode)
    {
      #include "opcodes.h"
    }
  }
  printf("\n--------\n");
  print_registers(Z);
}

void print_registers(cpu *Z)
{
  printf("AF: 0x%x\n",_AF);
  printf("BC: 0x%x\n",_BC);
  printf("DE: 0x%x\n",_DE);
  printf("HL: 0x%x\n",_HL);
  printf("PC: 0x%x\n",_PC);
  printf("SP: 0x%x\n",_SP);
}

void debug_mode(cpu *Z)
{
  char command[4];
  for(;;)
  {
    printf("@");
    fgets(command,6,stdin);
    scanf("%[^\n]",command);
    if (!strcmp(command,"p")) {print_registers(Z);}
    else if (!strcmp(command,"h")) {write_word(_PC-2,0xDEFD);}
    else if (!strcmp(command,"q")) {break;}
    else if (!strcmp(command,"imm")) {printf("IMM8: 0x%x\nIMM16: 0x%x\n",IMM8,IMM16);}
    else if (!strcmp(command,"stack")) {printf("STACK: [0x%x] = 0x%x\n",_SP,read_word(_SP));}
    else if (command[0] == 'm')
    {
      uint16 address;
      char addrl[2];
      char addrh[2];
      addrl[0] = command[2];
      addrl[1] = command[3];
      addrh[0] = command[4];
      addrh[1] = command[5];
      address = convert_hex(addrh)+(convert_hex(addrl) << 8);
      printf("MEM: [0x%x] = 0x%x\n",address,read_byte(address));
    }
    else if (command[0] == 't')
    {
      char opcode[2];
      opcode[0] = command[2];
      opcode[1] = command[3];
      test_opcode(convert_hex(opcode),1,Z);
    }
    else if (command[0] == 'c' && command[1] == 'b')
    {
      char opcode[2];
      opcode[0] = command[3];
      opcode[1] = command[4];
      test_opcode(convert_hex(opcode),0xCB,Z);
    }
  }
}

uint8 convert_hex(char *c)
{
  uint8 x = 0;
  if (c[0] < 103 && c[0] > 96) //c[0] is a lowercase letter
  {
    x |= (c[0] - 'a' + 10) << 4;
  }
  else if (c[0] < 71 && c[0] > 64)//c[0] is an uppercase letter
  {
    x |= (c[0] - 'A' + 10) << 4;
  }
  else if (c[0] < 58 && c[0] > 47)//c[0] is a number
  {
    x |= (c[0] - '0') << 4;
  }
  if (c[1] < 103 && c[1] > 96) //c[1] is a lowercase letter
  {
    x |= (c[1] - 'a' + 10);
  }
  else if (c[1] < 71 && c[1] > 64)//c[1] is an uppercase letter
  {
    x |= (c[1] - 'A' + 10);
  }
  else if (c[1] < 58 && c[1] > 47)//c[1] is a number
  {
    x |= (c[1] - '0');
  }
  return x;
}

uint16 handle_args(int argc, char *argv[])
{
  if (argc < 2) {return 0;}
  int i;
  for (i = 1; i < argc; i++)
  {
    if (!strcmp(argv[i],"-h")) {return 0;}
    else if (!strcmp(argv[i],"-i")) {return 2;}
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
    else if (!strcmp(argv[i],"-r")) {return 1;}
  }
}
