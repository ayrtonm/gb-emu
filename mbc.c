#include "mbc.h"

void write_ram(uint16 address, uint8 value)
{
  if (_ERAM <= address < _WRAM)
  {
    MEM(address) = value;
  }
}

void write_mbc1(uint16 address, uint8 value)
{
  if (address < 0x2000)
  {
    ENABLE = ((value & 0x0F) == 0x0A) ? 1 : 0;
  }
  else if (address < 0x4000)
  {
    ROMBANK.B.l = value & 0x1F;
    if (ROMBANK.B.l == 0)
    {
      ROMBANK.B.l = 0x01;
    }
    ROMBANK.W &= 0x031F;
    int i;
    for (i = _ROM; i < _BANK; i++)
    {
      MEM(i + _BANK) = CART(i+(ROMBANK.B.l + (ROMBANK.B.h << 5))*0x4000);
    }
  }
  else if (address < 0x6000)
  {
    if (MODE == 0)
    {
      ROMBANK.B.h = value & 0x03;
      ROMBANK.W &= 0x031F;
      int i;
      for (i = _ROM; i < _BANK; i++)
      {
        MEM(i + _BANK) = CART(i + (ROMBANK.B.l + (ROMBANK.B.h << 5))*0x4000);
      }
    }
    else if (MODE == 1)
    {
      uint8 oldbank = RAMBANK;
      RAMBANK = value & 0x03;
      int i;
      for (i = _ERAM; i < _WRAM; i++)
      {
        ERAM((i - _ERAM) + oldbank*0x2000) = MEM(i);
        MEM(i) = ERAM((i - _ERAM) + RAMBANK*0x2000);
      }
    }
  }
  else if (address < 0x8000)
  {
    MODE = value & 0x01;
  }
  else if (_ERAM <= address < _WRAM)
  {
    MEM(address) = (ENABLE == 1) ? value : MEM(address);
  }
}

void write_mbc3(uint16 address, uint8 value)
{
  if (address < 0x2000)
  {
    ENABLE = ((value & 0x0F) == 0x0A) ? 1 : 0;
  }
  else if (address < 0x4000)
  {
    ROMBANK.B.l = value & 0x7F;
    if (ROMBANK.B.l == 0)
    {
      ROMBANK.B.l = 0x01;
    }
    ROMBANK.W &= 0x007F;
    int i;
    for (i = _ROM; i < _BANK; i++)
    {
      MEM(i + _BANK) = CART(i+ROMBANK.B.l*0x4000);}
    }
  else if (address < 0x6000)
  {
    if (value < 0x04)
    {
      uint8 oldbank = RAMBANK;
      RAMBANK = value & 0x03;
      int i;
      for (i = _ERAM; i < _WRAM; i++)
      {
        ERAM((i - _ERAM) + oldbank*0x2000) = MEM(i);
        MEM(i) = ERAM((i - _ERAM) + RAMBANK*0x2000);}
      }
    else if (value < 0x0C)//map corresponding rtc register to memory at a000-bfff, rtc register can be accessed by writing to any address in range
    {
      return;
     }
  }
  else if (address < 0x8000)//latch clock data by writing 0x00 then 0x01
  {
    return;
  }
  else if (_ERAM <= address < _WRAM)
  {
    MEM(address) = (ENABLE == 1) ? value : MEM(address);
  }
}
void write_mbc5(uint16 address, uint8 value)
{
  if (address < 0x2000)
  {
    ENABLE = ((value & 0x0F) == 0x0A) ? 1 : 0;
  }
  else if (address < 0x3000)
  {
    ROMBANK.B.l = value;
    ROMBANK.W &= 0x01FF;
    int i;
    for (i = _ROM; i < _BANK; i++)
    {
      MEM(i + _BANK) = CART((i+ROMBANK.W*0x4000) & ((rom_sizes[CART(0x0148)] << 8) - 1));
    }
  }
  else if (address < 0x4000)
  {
    ROMBANK.B.h = value & 0x01;
    ROMBANK.W &= 0x01FF;
    int i;
    for (i = _ROM; i < _BANK; i++)
    {
      MEM(i + _BANK) = CART((i+ROMBANK.W*0x4000) & ((rom_sizes[CART(0x0148)] << 8) - 1));
    }
  }
  else if (address < 0x6000)
  {
    uint8 oldbank = RAMBANK;
    RAMBANK = value & 0x0F;
    int i;
    for (i = _ERAM; i < _WRAM; i++)
    {
      ERAM((i-_ERAM) + oldbank*0x2000) = MEM(i);
      MEM(i) = ERAM((i - _ERAM) + RAMBANK*0x2000);
    }
  }
  else if (address < 0x8000)
  {
    return;
  }
  else if (_ERAM <= address < _WRAM)
  {
    MEM(address) = (ENABLE == 1) ? value : MEM(address);
  }
}
