#include <stdio.h>
#include <string.h>
#include <math.h>

#define HELP_TEXT "Usage: ./tool [FLAGS] > FILE\nFlags\n -cb  CB opcodes\n -n   normal opcodes\n -h   display this text\n"
/*
  makes the inside of a switch(case) statement with all possibilities for a uint8 variable
  now includes cb opcodes 0x40 through 0xFF
  added math.h for trunc() to help with patterns
  09/13 - fixed mem[HL] opcodes by adding exceptions for opcodes 0x80 - 0xFF and with read_byte and write_byte instructions
  now includes ALL cb opcodes
*/
int main(int argc, char *argv[])
{
  if (argc < 2) {printf(HELP_TEXT);return 0;}
  int i;
  char *registers[8] = {"_B","_C","_D","_E","_H","_L","_HL","_A",};
  char *byte_registers[8] = {"_B","_C","_D","_E","_H","_L","read_byte(_HL)","_A",};
  char *word_registers[4] = {"_BC","_DE","_HL","_SP"};
  if (!strcmp(argv[1],"-cb"))
  {
    for (i=0x00;i<0x08;i++)
    {
      if ((i & 0x0F) == 0x06 || (i & 0x0F) == 0x0E)
        {printf("case 0x%x:  {uint8 temp = read_byte(_HL);RLC(temp);write_byte(_HL,temp);break;}\n",i);}
      else
        {printf("case 0x%x:  {RLC(%s);break;}\n",i,registers[i & 0x07]);}
    }
    for (i=0x08;i<0x10;i++)
    {
      if ((i & 0x0F) == 0x06 || (i & 0x0F) == 0x0E)
        {printf("case 0x%x:  {uint8 temp = read_byte(_HL);RRC(temp);write_byte(_HL,temp);break;}\n",i);}
      else
        {printf("case 0x%x:  {RRC(%s);break;}\n",i,registers[i & 0x07]);}
    }
    for (i=0x10;i<0x18;i++)
    {
      if ((i & 0x0F) == 0x06 || (i & 0x0F) == 0x0E)
        {printf("case 0x%x:  {uint8 temp = read_byte(_HL);RL(temp);write_byte(_HL,temp);break;}\n",i);}
      else
        {printf("case 0x%x:  {RL(%s);break;}\n",i,registers[i & 0x07]);}
    }
    for (i=0x18;i<0x20;i++)
    {
      if ((i & 0x0F) == 0x06 || (i & 0x0F) == 0x0E)
        {printf("case 0x%x:  {uint8 temp = read_byte(_HL);RR(temp);write_byte(_HL,temp);break;}\n",i);}
      else
        {printf("case 0x%x:  {RR(%s);break;}\n",i,registers[i & 0x07]);}
    }
    for (i=0x20;i<0x28;i++)
    {
      if ((i & 0x0F) == 0x06 || (i & 0x0F) == 0x0E)
        {printf("case 0x%x:  {uint8 temp = read_byte(_HL);SLA(temp);write_byte(_HL,temp);break;}\n",i);}
      else
        {printf("case 0x%x:  {SLA(%s);break;}\n",i,registers[i & 0x07]);}
    }
    for (i=0x28;i<0x30;i++)
    {
      if ((i & 0x0F) == 0x06 || (i & 0x0F) == 0x0E)
        {printf("case 0x%x:  {uint8 temp = read_byte(_HL);SRA(temp);write_byte(_HL,temp);break;}\n",i);}
      else
        {printf("case 0x%x:  {SRA(%s);break;}\n",i,registers[i & 0x07]);}
    }
    for (i=0x30;i<0x38;i++)
    {
      if ((i & 0x0F) == 0x06 || (i & 0x0F) == 0x0E)
        {printf("case 0x%x:  {uint8 temp = read_byte(_HL);CB_SWAP(temp);write_byte(_HL,temp);break;}\n",i);}
      else
        {printf("case 0x%x:  {CB_SWAP(%s);break;}\n",i,registers[i & 0x07]);}
    }
    for (i=0x38;i<0x40;i++)
    {
      if ((i & 0x0F) == 0x06 || (i & 0x0F) == 0x0E)
        {printf("case 0x%x:  {uint8 temp = read_byte(_HL);SRL(temp);write_byte(_HL,temp);break;}\n",i);}
      else
        {printf("case 0x%x:  {SRL(%s);break;}\n",i,registers[i & 0x07]);}
    }
    for (i=0x40;i<0x80;i++)
    {
      if ((i & 0x0F) == 0x06 || (i & 0x0F) == 0x0E)
        {printf("case 0x%x:  {(GET(0x%x,read_byte(_HL)) == 0 ? SET(Z_FLAG,_F) : CLEAR(Z_FLAG,_F));SET(H_FLAG,_F);CLEAR(N_FLAG,_F);break;}\n",i,(1 << (int)trunc((i-0x40)/0x08)));}
      else
        {printf("case 0x%x:  {(GET(0x%x,%s) == 0 ? SET(Z_FLAG,_F) : CLEAR(Z_FLAG,_F));SET(H_FLAG,_F);CLEAR(N_FLAG,_F);break;}\n",i,(1 << (int)trunc((i-0x40)/0x08)),registers[i & 0x07]);}
    }
    for (i=0x80;i<0xC0;i++)
    {
      if ((i & 0x0F) == 0x06 || (i & 0x0F) == 0x0E)
        {printf("case 0x%x:  {uint8 temp = read_byte(_HL);CLEAR(0x%x,temp);write_byte(_HL,temp);break;}\n",i,(1 << (int)trunc((i-0x80)/0x08)));}
      else
        {printf("case 0x%x:  {CLEAR(0x%x,%s);break;}\n",i,(1 << (int)trunc((i-0x80)/0x08)),registers[i & 0x07]);}
    }
    for (i=0xC0;i<0x0100;i++)
    {
      if ((i & 0x0F) == 0x06 || (i & 0x0F) == 0x0E)
        {printf("case 0x%x:  {uint8 temp = read_byte(_HL);SET(0x%x,temp);write_byte(_HL,temp);break;}\n",i,(1 << (int)trunc((i-0xC0)/0x08)));}
      else
        {printf("case 0x%x:  {SET(0x%x,%s);break;}\n",i,(1 << (int)trunc((i-0xC0)/0x08)),registers[i & 0x07]);}
    }
    printf("default:  {printf(\"CB opcode 0x%s not found\\n\",opcode);getchar();}\n","%x");return 0;
  }
  else if (!strcmp(argv[1],"-n"))
  {
    for (i=0x00;i<0x40;i++)
    {
      if ((i & 0x0F) == 0x03)
        {printf("case 0x%x:  {%s++;break;}\n",i,word_registers[(i & 0xF0)>>4]);}
      else if ((i & 0x0F) == 0x0B)
        {printf("case 0x%x:  {%s--;break;}\n",i,word_registers[(i & 0xF0)>>4]);}
      else
        {printf("case 0x%x:  {break;}\n",i);}
    }
    for (i=0x40;i<0x80;i++)
    {
      printf("case 0x%x:  {LD(%s,%s);break;}\n",i,registers[((i & 0xF8)-0x40)/8],registers[i & 0x07]);
    }
    for (i=0x80;i<0x88;i++)
    {
      printf("case 0x%x:  {ADD(%s);break;}\n",i,byte_registers[i & 0x07]);
    }
    for (i=0x88;i<0x90;i++)
    {
      printf("case 0x%x:  {ADC(%s);break;}\n",i,byte_registers[i & 0x07]);
    }
    for (i=0x90;i<0x98;i++)
    {
      printf("case 0x%x:  {SUB(%s);break;}\n",i,byte_registers[i & 0x07]);
    }
    for (i=0x98;i<0xA0;i++)
    {
      printf("case 0x%x:  {SBC(%s);break;}\n",i,byte_registers[i & 0x07]);
    }
    for (i=0xA0;i<0xA8;i++)
    {
      printf("case 0x%x:  {AND(%s);break;}\n",i,byte_registers[i & 0x07]);
    }
    for (i=0xA8;i<0xB0;i++)
    {
      printf("case 0x%x:  {EOR(%s);break;}\n",i,byte_registers[i & 0x07]);
    }
    for (i=0xB0;i<0xB8;i++)
    {
      printf("case 0x%x:  {OR(%s);break;}\n",i,byte_registers[i & 0x07]);
    }
    for (i=0xB8;i<0xC0;i++)
    {
      printf("case 0x%x:  {CP(%s);break;}\n",i,byte_registers[i & 0x07]);
    }
    for (i=0xC0;i<0x0100;i++)
    {
      if ((i & 0x0F) == 0x01 && i > 0xC0)
        {printf("case 0x%x:  {POP(%s);break;}\n",i,word_registers[(i & 0x30)>>4]);}
      else if ((i & 0x0F) == 0x05 && i > 0xC0)
        {printf("case 0x%x:  {PUSH(%s);break;}\n",i,word_registers[(i & 0x30)>>4]);}
      else
        {printf("case 0x%x:  {break;}\n",i);}
    }
    printf("default:  {printf(\"Opcode 0x%s not found\\n\",opcode);getchar();}\n","%x");
    printf("/*The normal opcodes header file has been modified\nyou probably don't want to replace it with this text\nand this part of the program will probably never be complete\ndue to a lack of patterns in the regular gameboy opcodes*/\n");
    return 0;
  }
  else if (!strcmp(argv[1],"-h"))
    {printf(HELP_TEXT);return 0;}
}
