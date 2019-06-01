#include "mem.h"
#include "bits.h"
#include <iostream>
#include <fstream>
#undef DEBUG_INTERNAL
#undef DEBUG
#undef DEBUG_ROMBANK
#undef DEBUG_RAMBANK

using namespace std;

mem::mem(string filename, string memorydump) {
  if (memorydump != "") {
    dumpmemory = true;
    memorydumpfile = memorydump;
  }
  load_cart(filename);
  memory[O_IO + IO_TIMA] = 0x00;
  memory[O_IO + IO_TMA] = 0x00;
  memory[O_IO + IO_TAC] = 0x00;
  memory[O_IO + IO_NR10] = 0x80;
  memory[O_IO + IO_NR11] = 0xbf;
  memory[O_IO + IO_NR12] = 0xf3;
  memory[O_IO + IO_NR14] = 0xbf;
  memory[O_IO + IO_NR21] = 0x3f;
  memory[O_IO + IO_NR22] = 0x00;
  memory[O_IO + IO_NR24] = 0xbf;
  memory[O_IO + IO_NR30] = 0x7f;
  memory[O_IO + IO_NR31] = 0xff;
  memory[O_IO + IO_NR32] = 0x9f;
  memory[O_IO + IO_NR33] = 0xbf;
  memory[O_IO + IO_NR41] = 0xff;
  memory[O_IO + IO_NR42] = 0x00;
  memory[O_IO + IO_NR43] = 0x00;
  memory[O_IO + IO_NR44] = 0xbf;
  memory[O_IO + IO_NR50] = 0x77;
  memory[O_IO + IO_NR51] = 0xf3;
  memory[O_IO + IO_NR52] = 0xf1;
  memory[O_IO + IO_LCDC] = 0x91;
  memory[O_IO + IO_SCY] = 0x00;
  memory[O_IO + IO_SCX] = 0x00;
  memory[O_IO + IO_LYC] = 0x00;
  memory[O_IO + IO_BGP] = 0xfc;
  memory[O_IO + IO_OBP0] = 0xff;
  memory[O_IO + IO_OBP1] = 0xff;
  memory[O_IO + IO_WY] = 0x00;
  memory[O_IO + IO_WX] = 0x00;
  memory[O_IE] = 0x00;
  
  memory[O_IO + IO_LCDSTAT] = 0x91;

  ramenabled = false;
  mbcmode = rom;
  current_rombank = 1;
  current_rambank = 0;

  divtimer = 0;
  timatimer = 0;
  tacthreshold = tacvals[0];
  dmatimer = 0;
  dmatransfering = false;
  //0x0f is no keys pressed
  joydirection = 0x0f;
  joyspecial = 0x0f;
  //initiallize joypad register to something sensible
  loadeddirection = true;
  //direction is actually selected since setting the bit disables it
  memory[O_IO + IO_JOYP] = (JOYP_SPECIAL_SELECTED|0x0f);
}
void mem::write_byte_internal(uint16 address, uint8 data) {
#ifdef DEBUG_INTERNAL
  cout << "called mem::write_byte_internal(0x" << hex << (int)address << ", 0x" << hex << (int)data << ")" << endl;
#endif
  //this can only take values between 0 and 153 but step_lcd takes care of that
  if (address == O_IO+IO_LY) {
    memory[O_IO+IO_LY] = data;
  }
  else if (address == O_IO+IO_LCDSTAT) {
    memory[O_IO+IO_LCDSTAT] = data;
  }
  else {
    memory[address] = data;
  }
}
void mem::write_byte(uint16 address, uint8 data) {
#ifdef DEBUG
  cout << "called mem::write_byte(0x" << hex << (int)address << ", 0x" << hex << (int)data << ")" << endl;
#endif
  //writing to anything but HRAM is restricted during DMA transfer
  if (dmatransfering) {
    if ((address >= O_HRAM) && (address != O_IE)) {
      memory[address] = data;
    }
    return;
  }
  //writing to OAM and VRAM is restricted during certain LCD modes
  //this doesn't give good results yet
  //lcdmode is memory[O_IO + IO_LCDSTAT] & 0x03
  //else if (((address < O_UNUSED) && (address >= O_OAM)) && ((lcdmode == 2) || (lcdmode == 3))) {
  //  //can't access OAM during lcd modes 2 and 3
  //  return;
  //}
  //else if (((address < O_ERAM) && (address >= O_VRAM)) && (lcdmode == 3)) {
  //  //can't access VRAM during lcd mode 3
  //  return;
  //}
  else if (address == O_IO+IO_LCDSTAT) {
    //lower 3 bits of LCD STAT are read only
    memory[O_IO+IO_LCDSTAT] = (memory[O_IO+IO_LCDSTAT] & 0x07) | (data & 0xf8);
    return;
  }
  else if (address == O_IO+IO_LY) {
    //this is read only
    return;
  }
  else if (address == O_IO+IO_BGP) {
    memory[address] = data;
    update_palette(2, data);
    return;
  }
  else if (address == O_IO+IO_OBP0) {
    memory[address] = data;
    update_palette(0, data);
    return;
  }
  else if (address == O_IO+IO_OBP1) {
    memory[address] = data;
    update_palette(1, data);
    return;
  }
  else if (address == O_IO+IO_LY) {
    //this is read only, attempting to write to it "resets the counter"
    //memory[O_IO+IO_LY] = 0;
    return;
  }
  //writing to sound registers is only allowed if sound is enabled in NR52
  //this restriction doesn't apply to NR52
  //only implementing appropriate read/write permissions and side effects for now (i.e. reading/writing won't actually produce sound)
  else if (address == O_IO + IO_NR10) {
    if (memory[O_IO + IO_NR52] & 0x80) {
      memory[address] = data;
    }
    return;
  }
  //bits 6-7 are read/write, other bits are write only and produce side effects
  else if (address == O_IO + IO_NR11) {
    if (memory[O_IO + IO_NR52] & 0x80) {
      //since lower 6 bits are write only, store the data somewhere else so read_byte() will return 0 for these bits
      ch1t = data & 0x3f;
      memory[address] = data & 0xc0;
    }
    return;
  }
  //read/write with side effects
  else if (address == O_IO + IO_NR12) {
    if (memory[O_IO + IO_NR52] & 0x80) {
      memory[address] = data;
    }
    return;
  }
  //write only with side effects
  else if (address == O_IO + IO_NR13) {
    if (memory[O_IO + IO_NR52] & 0x80) {
      memory[address] = 0;
    }
    return;
  }
  //can only read from bit 6, bits 0-2 and bit 7 are write only with side effects
  else if (address == O_IO + IO_NR14) {
    if (memory[O_IO + IO_NR52] & 0x80) {
      //if writing to bit 7, set flag in NR52
      if (data & 0x80) {
        memory[O_IO+IO_NR52] |= 0x01;
        ch1timer = (64 - ch1t)*3906;
      }
      memory[address] = data & 0x40;
    }
    return;
  }
  //bits 6-7 are read/write, bits 0-5 are write only with side effects
  else if (address == O_IO + IO_NR21) {
    if (memory[O_IO + IO_NR52] & 0x80) {
      //since lower 6 bits are write only, store the data somewhere else so read_byte() will return 0 for these bits
      ch2t = data & 0x3f;
      memory[address] = data & 0xc0;
    }
    return;
  }
  //read/write with side effects
  else if (address == O_IO + IO_NR22) {
    if (memory[O_IO + IO_NR52] & 0x80) {
      memory[address] = data;
    }
    return;
  }
  //write only with side effects
  else if (address == O_IO + IO_NR23) {
    if (memory[O_IO + IO_NR52] & 0x80) {
      memory[address] = 0;
    }
    return;
  }
  //can only read from bit 6, bits 0-2 and bit 7 are write only with side effects
  else if (address == O_IO + IO_NR24) {
    if (memory[O_IO + IO_NR52] & 0x80) {
      //if writing to bit 7, set flag in NR52
      if (data & 0x80) {
        memory[O_IO+IO_NR52] |= 0x02;
        ch2timer = (64 - ch2t)*3906;
      }
      memory[address] = data & 0x40;
    }
    return;
  }
  //bit 7 is read/write with side effects
  else if (address == O_IO + IO_NR30) {
    if (memory[O_IO + IO_NR52] & 0x80) {
      memory[address] = data & 0x80;
    }
    return;
  }
  //permissions not specified in pandocs
  //possibly a timer/counter
  else if (address == O_IO + IO_NR31) {
    if (memory[O_IO + IO_NR52] & 0x80) {
      //since lower 6 bits are write only, store the data somewhere else so read_byte() will return 0 for these bits
      ch3t = data;
      //pandocs doesn't specify behavior of writing to NR31 so I'm following same pattern in NR11 and NR21
      //memory[address] = data;
    }
    return;
  }
  //bits 5-6 are read/write with side effects
  else if (address == O_IO + IO_NR32) {
    if (memory[O_IO + IO_NR52] & 0x80) {
      memory[address] = data & 0x60;
    }
    return;
  }
  //write only with side effects
  else if (address == O_IO + IO_NR33) {
    if (memory[O_IO + IO_NR52] & 0x80) {
      memory[address] = 0;
    }
    return;
  }
  //can only read from bit 6, bits 0-2 and bit 7 are write only with side effects
  else if (address == O_IO + IO_NR34) {
    if (memory[O_IO + IO_NR52] & 0x80) {
      //if writing to bit 7, set flag in NR52
      if (data & 0x80) {
        memory[O_IO+IO_NR52] |= 0x04;
        ch3timer = (256 - ch3t)*3906;
      }
      memory[address] = data & 0x40;
    }
    return;
  }
  //bits 0-5 are read/write with side effects
  else if (address == O_IO + IO_NR41) {
    if (memory[O_IO + IO_NR52] & 0x80) {
      //since lower 6 bits are write only, store the data somewhere else so read_byte() will return 0 for these bits
      ch4t = data & 0x3f;
      //pandocs doesn't specify behavior of writing to NR31 so I'm following same pattern in NR11 and NR21
      memory[address] = data & 0xc0;
    }
    return;
  }
  //read/write with side effects
  else if (address == O_IO + IO_NR42) {
    if (memory[O_IO + IO_NR52] & 0x80) {
      memory[address] = data;
    }
    return;
  }
  //read/write with side effects
  else if (address == O_IO + IO_NR43) {
    if (memory[O_IO + IO_NR52] & 0x80) {
      memory[address] = data;
    }
    return;
  }
  //bit 6 is read/write, bit 7 is write only with side effects
  else if (address == O_IO + IO_NR44) {
    if (memory[O_IO + IO_NR52] & 0x80) {
      //if writing to bit 7, set flag in NR52
      if (data & 0x80) {
        ch4timer = (64 - ch4t)*3906;
        memory[O_IO+IO_NR52] |= 0x08;
      }
      memory[address] = data & 0x40;
    }
    return;
  }
  //read/write with side effects
  else if (address == O_IO + IO_NR50) {
    if (memory[O_IO + IO_NR52] & 0x80) {
      memory[address] = data;
    }
    return;
  }
  //read/write with side effects
  else if (address == O_IO + IO_NR51) {
    if (memory[O_IO + IO_NR52] & 0x80) {
      memory[address] = data;
    }
    return;
  }
  //bit 7 is read/write, bits 0-3 are read only
  else if (address == O_IO + IO_NR52) {
    //writing zero to bit 7 destroys the contents of all sound registers
    if ((data & 0x80) == 0x00) {
      for (uint8 i = IO_NR10; i <= IO_NR52; i++) {
        //sound registers are contiguous memory from NR10 to NR52 except 0xFF15 and 0xFF1F
        if (i != 0x15 && i != 0x1f) {
          memory[O_IO + i] = 0;
        }
      }
    }
    memory[address] = data & 0x80;
    return;
  }
  else if (address == O_IO+IO_DIV) {
    memory[O_IO+IO_DIV] = 0x00;
    divtimer = 0;
    return;
  }
  else if (address == O_IO+IO_TAC) {
    tacthreshold = tacvals[data & 0x03];
    timatimer = 0;
    memory[O_IO+IO_TAC] = data;
    return;
  }
  else if (address == O_IO+IO_JOYP) {
    uint8 keysselected = data & 0x30;
    //if special keys are selected
    if (keysselected == JOYP_DIRECTION_SELECTED) {
      loadeddirection = false;
      memory[O_IO+IO_JOYP] = (joyspecial & 0x0f)|(data & 0xf0);
      return;
    }
    //if direction keys are selected
    else if (keysselected == JOYP_SPECIAL_SELECTED) {
      loadeddirection = true;
      memory[O_IO+IO_JOYP] = (joydirection & 0x0f)|(data & 0xf0);
      return;
    }
    //if neither set is selected
    else if (keysselected == (JOYP_DIRECTION_SELECTED|JOYP_SPECIAL_SELECTED)) {
      //keep previous setting for loadeddirection
      memory[O_IO+IO_JOYP] = (data & 0xf0);
      return;
    }
    //if both sets are selected (undefined behavior)
    else if (keysselected == 0x00) {
      //ignore attempt to write to memory
      memory[O_IO+IO_JOYP] = data;
      return;
    }
  }
  else if (address == O_IO+IO_DMA) {
    if (data <= 0xF1) {
      dmatimer = 160;
      dmatransfering = true;
      //all data is written immediately after the DMA is executed which very likely does not match the hardware's behavior
      //this should not matter since accessable memory is limited during DMA transfer
      for (int i = 0; i < 160; i++) {
        memory[0xFE00 + i] = memory[(data << 8) + i];
      }
    }
    return;
  }
  else if (address < 0xC000) {
    //explicitly dereferencing handle_mbc to highlight that this is a pointer
    (*this.*handle_mbc)(address, data);
    return;
  }
  else {
    memory[address] = data;
    return;
  }
};

//the handle MBC functions need to access and modify the memory model so they should be members of the mem class
//the ROM only handle function should take care of writing to RAM
void mem::handle_romonly(uint16 address, uint8 data) {
};
void mem::handle_mbc2(uint16 address, uint8 data) {
};
void mem::handle_mbc3(uint16 address, uint8 data) {
};
void mem::handle_mbc1(uint16 address, uint8 data) {
  //writing to this part of ROM enables RAM if an MBC-type circuit is present
  if (address < 0x2000) {
    if (num_rambanks != 0) {
      if (data & 0x0A) {
        ramenabled = true;
      }
      else {
        ramenabled = false;
      }
    }
    return;
  }
  //writing to this part of ROM selects the ROM bank number if an MBC is present
  else if (address < 0x4000) {
    if (mbctype == mbc1 && num_rombanks != 0) {
      data &= 0x1f;
      if (data == 0x00) {
        data = 0x01;
      }
      //combine new data with current rombank and update addressable memory
      switch_rombanks(data|(current_rombank & 0x60));
    }
    else if (mbctype == mbc3 && num_rombanks != 0) {
      data &= 0x7f;
      if (data == 0x00) {
        data = 0x01;
      }
      //combine new data with current rombank and update addressable memory
      switch_rombanks(data);
    }
    return;
  }
  //writing to this part of ROM selects the RAM bank number or the upper bits of ROM bank number if an MBC is present
  else if (address < 0x6000) {
    if (mbctype == mbc1) {
      if (mbcmode == rom) {
        data &= 0x03;
        //combine new data with current rombank and update addressable memory
        switch_rombanks(data|(current_rombank & 0x1f));
      }
      else if (mbcmode == ram && ramenabled) {
      }
    }
    else if (mbctype == mbc3) {
      if (ramenabled) {
        if (data < 0x04) {
          mbcmode = ram;
          switch_rambanks(data);
        }
        else if (data > 0x07 && data < 0x0d) {
          mbcmode = rtc;
          current_rtc =  data - 0x08;
        }
      }
    }
    return;
  }
  //writing to this part of ROM selects between ROM and RAM mode if an MBC is present
  else if (address < 0x8000) {
    if (mbctype == mbc1) {
      mbcmode = (data == 0x01 ? ram : rom);
    }
    return;
  }
  else if (address < 0xA000) {
    memory[address] = data;
    return;
  }
  else if (address < 0xC000) {
    if (mbctype == mbc1) {
    }
    else if (mbctype == mbc3) {
      if (mbcmode == ram) {
        memory[address] = data;
        return;
      }
      else if (mbcmode == rtc) {
        rtc_registers[current_rtc] = data;
      }
    }
    else {
      memory[address] = data;
      return;
    }
  }
};

void mem::switch_rombanks(int newbank) {
#ifdef DEBUG_ROMBANK
  cout << "switching to ROM bank " << newbank << endl;
#endif
  uint8 temp;
  for (int i = 0; i < 0x4000; i++) {
    temp = memory[0x4000 + i];
    memory[0x4000 + i] = rombanks[newbank][i];
    rombanks[current_rombank][i] = temp;
  }
  current_rombank = newbank;
};
void mem::switch_rambanks(int newbank) {
#ifdef DEBUG_RAMBANK
  cout << "switching to RAM bank " << newbank << endl;
#endif
  uint8 temp;
  for (int i = 0; i < 0x2000; i++) {
    temp = memory[0xA000 + i];
    memory[0xA000 + i] = rambanks[newbank][i];
    rambanks[current_rambank][i] = temp;
  }
  current_rambank = newbank;
};

void mem::load_cart(string filename) {
  streampos size;
  ifstream cart;
  cart.exceptions(ifstream::failbit | ifstream::badbit);
  try {
    //open file with pointer positioned at the end
    //file is a binary and set the initial position at the end of the file to get its size
    cart.open(filename, ios::binary|ios::ate);
    //if we can't manage to open the file
    //I should signal to main() to return with an error code from here
    //if (!cart.is_open()) {
    //  cout << "unable to open " << filename << ": no cartridge loaded" << endl;
    //  //execute halt instruction if no file is loaded (temporary)
    //  //memory[0x0100] = 0x76;
    //  return;
    //}
    size = cart.tellg();
    //rombn.resize((int)size - (int)O_ROMBN);
    cart.seekg(0,ios::beg);
    cart.read((char *) &memory[0],0x8000);
    //cart.read((char *) &romb0[0],O_ROMBN);
    //cart.read((char *) &rombn[0],(int)size-(int)O_ROMBN);
  }
  catch (const ifstream::failure &e) {
    cout << "exception opening ROM\n";
  }
  cart.close();
  for (int i =0x0134; i < 0x0144; i++) cout << read_byte(i);//print title
  cout << (read_byte(0x014A) ? "\nNon-Japanese" : "\nJapanese");
  cout << "\n";
  try {
    switch(read_byte(0x0147)) {
      case 0x00: {mbctype = romonly;break;}

      case 0x01: {mbctype = mbc1;break;}
      case 0x02: {mbctype = mbc1;break;}
      case 0x03: {mbctype = mbc1;break;}

      case 0x05: {mbctype = mbc2;break;}
      case 0x06: {mbctype = mbc2;break;}
      //0x07 undefined
      case 0x08: {mbctype = romonly;break;}
      case 0x09: {mbctype = romonly;break;}

      case 0x0b: {mbctype = mm01;break;}
      case 0x0c: {mbctype = mm01;break;}
      case 0x0d: {mbctype = mm01;break;}

      case 0x0f: {mbctype = mbc3;break;}
      case 0x10: {mbctype = mbc3;break;}
      case 0x11: {mbctype = mbc3;break;}
      case 0x12: {mbctype = mbc3;break;}
      case 0x13: {mbctype = mbc3;break;}
      //0x14 undefined
      case 0x15: {mbctype = mbc4;break;}
      case 0x16: {mbctype = mbc4;break;}
      case 0x17: {mbctype = mbc4;break;}
      //0x18 undefined
      case 0x19: {mbctype = mbc5;break;}
      case 0x1a: {mbctype = mbc5;break;}
      case 0x1b: {mbctype = mbc5;break;}
      case 0x1c: {mbctype = mbc5;break;}
      case 0x1d: {mbctype = mbc5;break;}
      case 0x1e: {mbctype = mbc5;break;}

      case 0xfe: {mbctype = huc3;break;}
      case 0xff: {mbctype = huc1;break;}
      default: {
        throw runtime_error("cartridge includes unknown MBC");
        break;
      }
    }
    switch(read_byte(0x0148)) {
      case 0x00: {num_rombanks = 0;break;}
      case 0x01: {num_rombanks = 4;break;}
      case 0x02: {num_rombanks = 8;break;}
      case 0x03: {num_rombanks = 16;break;}
      case 0x04: {num_rombanks = 32;break;}
      case 0x05: {num_rombanks = 64;break;} //MBC1 only uses 63 banks, but let's allocate 64 to simplify addressing
      case 0x06: {num_rombanks = 128;break;} //MBC1 only uses 125 banks, but let's allocate 128 to simplify addressing
      case 0x07: {num_rombanks = 256;break;}
      case 0x52: {num_rombanks = 72;break;}
      case 0x53: {num_rombanks = 80;break;}
      case 0x54: {num_rombanks = 96;break;}
      default: {
        throw runtime_error("cartridge includes unknown ROM size");
      }
    }
    switch(read_byte(0x0149)) {
      case 0x00: {num_rambanks = 0;break;}
      case 0x01: {num_rambanks = 1;break;} //only the first 2 kBytes are accessible in this case
      case 0x02: {num_rambanks = 1;break;}
      case 0x03: {num_rambanks = 4;break;}
      default: {
        throw runtime_error("cartridge includes unknown RAM size");
      }
    }
    uint8 checksum = 0;
    for (int i = 0x0134; i < 0x014d; i++) {
      checksum = checksum - memory[i] - 1;
    }
    if (checksum != memory[0x014d]) {
      throw runtime_error("header checksum failed");
    }
  }
  catch (const runtime_error &e) {
    cout << e.what();
  }
  //given what we just read, let's initialize the memory model as needed
  switch (mbctype) {
    case romonly: {handle_mbc = &mem::handle_romonly;break;}
    case mbc1: {handle_mbc = &mem::handle_mbc1;break;}
    case mbc2: {handle_mbc = &mem::handle_mbc2;break;}
    case mbc3: {handle_mbc = &mem::handle_mbc3;break;}
    //case mbc4: {handle_mbc = &mem::handle_mbc4;break;}
    //case mbc5: {handle_mbc = &mem::handle_mbc5;break;}
    //case huc3: {handle_mbc = &mem::handle_huc3;break;}
    //case huc1: {handle_mbc = &mem::handle_huc1;break;}
    //case mm01: {handle_mbc = &mem::handle_mm01;break;}
  }
  //increase ROM bank vector as needed
  if (mbctype != romonly && num_rombanks != 0) {
    rombanks.resize(num_rombanks);
  }
  if (num_rambanks != 0) {
    rambanks.resize(num_rambanks);
  }
}
void mem::update_palette(uint8 palette, uint8 value) {
  int j = 0;
  for (int i = 0x03; i < 0xff; i = i << 2) {
    switch ((value & i) >> (2*j)) {
      case 0: {palettes[palette][j] = {SDL_ALPHA_OPAQUE,0xcc,0xcc,0xcc};break;}
      case 1: {palettes[palette][j] = {SDL_ALPHA_OPAQUE,0x88,0x88,0x88};break;}
      case 2: {palettes[palette][j] = {SDL_ALPHA_OPAQUE,0x66,0x66,0x66};break;}
      case 3: {palettes[palette][j] = {SDL_ALPHA_OPAQUE,0x33,0x33,0x33};break;}
    }
    j++;
  }
}

array<color,4> mem::get_palette(uint8 palette_num) {
  return palettes[palette_num];
}
void mem::dump_memory() {
  ofstream dump;
  dump.open(memorydumpfile);
  for (uint16 i = 0; i < 0xffff; i++) {
    if (read_byte(i) != 0) {
      dump << "[0x" << hex << i << "]  0x" << hex << (int)read_byte(i) << endl;
    }
  }
  dump.close();
}
void mem::update_timers(int dt) {
  //compute increment based on time opcode takes to give a 16.384 kHz increment rate
  //increment every 256 CPU clicks
  //this should be independent of CPU throttling which is artificial
  divtimer += dt;
  if (divtimer > 64) {
    divtimer -= 64;
    write_byte_internal(O_IO+IO_DIV, read_byte(O_IO+IO_DIV)+1);
  }
  if (read_byte(O_IO+IO_TAC) & TIMER_ENABLED) {
    timatimer += dt;
    if (timatimer > tacthreshold) {
      timatimer -= tacthreshold;
      write_byte_internal(O_IO+IO_TIMA, read_byte(O_IO+IO_TIMA)+1);
      //if overflow load value in TMA register
      if (read_byte(O_IO+IO_TIMA) == 0x00) {
        write_byte_internal(O_IO+IO_TIMA, read_byte(O_IO+IO_TMA));
        //request a timer interrupt
        write_byte_internal(O_IO+IO_IR, read_byte(O_IO+IO_IR) | INT_TIM);
      }
    }
  }
  if (dmatimer != 0) {
    dmatimer -= dt;
    if (dmatimer <= 0) {
      dmatimer = 0;
      dmatransfering = false;
    }
  }
  if (ch1timer > 0) {
    ch1timer -= dt;
    if (ch1timer <= 0) {
      ch1timer = 0;
      //if 6th bit is set stop output when length in NR11 expires
      if (memory[O_IO + IO_NR14] & 0x40) {
        memory[O_IO + IO_NR52] &= 0x01;
      }
    }
  }
  if (ch2timer > 0) {
    ch2timer -= dt;
    if (ch2timer <= 0) {
      ch2timer = 0;
      //if 6th bit is set stop output when length in NR21 expires
      if (memory[O_IO + IO_NR24] & 0x40) {
        memory[O_IO + IO_NR52] &= 0x02;
      }
    }
  }
  if (ch3timer > 0) {
    ch3timer -= dt;
    if (ch3timer <= 0) {
      ch3timer = 0;
      //if 6th bit is set stop output when length in NR31 expires
      if (memory[O_IO + IO_NR34] & 0x40) {
        memory[O_IO + IO_NR52] &= 0x04;
      }
    }
  }
  if (ch4timer > 0) {
    ch4timer -= dt;
    if (ch4timer <= 0) {
      ch4timer = 0;
      //if 6th bit is set stop output when length in NR41 expires
      if (memory[O_IO + IO_NR44] & 0x40) {
        memory[O_IO + IO_NR52] &= 0x08;
      }
    }
  }
}

void mem::update_keys(keys k, uint8 bit, keypress kp) {
  if (k == special) {
    if (kp == down) {
      //if the key for a special button is pressed, clear that bit in joyspecial
      joyspecial &= ~bit;
    }
    else {
      //if the key for a special button is released, set that bit in joyspecial
      joyspecial |= bit;
    }
    //if joyspecial is currently loaded, update the lower 4 bits in memory
    if (!loadeddirection) {
      write_byte_internal(O_IO+IO_JOYP,(joyspecial & 0x0f) | (read_byte(O_IO+IO_JOYP) & 0xf0));
    }
  }
  else {
    if (kp == down) {
      //if the key for a direction button is pressed, clear that bit in joydirection
      joydirection &= ~bit;
    }
    else {
      //if the key for a direction button is released, set that bit in joydirection
      joydirection |= bit;
    }
    //if joydirection is currently loaded, update the lower 4 bits in memory
    if (loadeddirection) {
      write_byte_internal(O_IO+IO_JOYP,(joydirection & 0x0f) | (read_byte(O_IO+IO_JOYP) & 0xf0));
    }
  }
}
uint8 mem::get_keys(keys k) {
  return (k == special ? joyspecial : joydirection);
}
keys mem::get_keys_loaded() {
  return (loadeddirection ? direction : special);
}
