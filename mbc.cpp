#include "mem.h"
#include "bits.h"
#include <fstream>
#undef DEBUG_ROMBANK
#undef DEBUG_RAMBANK

//read contents of cartridge header
void mem::print_cart_info() {
  //print title
  for (int i =0x0134; i < 0x0144; i++) {
    cout << read_byte(i);
  }
  cout << endl;
  cout << (read_byte(0x0143) == 0xc0 ? "Color Game Boy mode only" : "non-Color Game Boy mode") << endl;
  cout << (read_byte(0x014A) ? "Non-Japanese" : "Japanese") << endl;
  switch(mbctype) {
    case romonly: {cout << "no MBC detected" << endl;break;}
    case mbc1: {cout << "MBC1 detected" << endl;break;}
    case mbc2: {cout << "MBC2 detected" << endl;break;}
    case mbc3: {cout << "MBC3 detected" << endl;break;}
    case mbc4: {cout << "MBC4 detected" << endl;break;}
    case mbc5: {cout << "MBC5 detected" << endl;break;}
    case huc1: {cout << "HUC1 detected" << endl;break;}
    case huc3: {cout << "HUC3 detected" << endl;break;}
    case mm01: {cout << "MM01 detected" << endl;break;}
  }
  cout << num_rombanks << " ROM banks detected" << endl;
  cout << num_rambanks << " RAM banks detected" << endl;
}

//load contents of cartridge into memory and ROM/RAM banks
//print cartridge info and return value based
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
    if (!cart.is_open()) {
      throw runtime_error("unable to open cartridge");
      return;
    }
    size = cart.tellg();
    cart.seekg(0,ios::beg);
    cart.read((char *) &memory[0],0x8000);
  }
  catch (const ifstream::failure &e) {
    cout << e.what() << endl;
    exit(1);
  }
  try {
    switch(read_byte(0x0147)) {
      case 0x00: {mbctype = romonly;break;}
      case 0x01: {mbctype = mbc1;break;}
      case 0x02: {mbctype = mbc1;break;}
      case 0x03: {mbctype = mbc1;break;}
      case 0x05: {mbctype = mbc2;break;}
      case 0x06: {mbctype = mbc2;break;}
      case 0x08: {mbctype = romonly;break;}
      case 0x09: {mbctype = romonly;break;}
      //case 0x0b: {mbctype = mm01;break;}
      //case 0x0c: {mbctype = mm01;break;}
      //case 0x0d: {mbctype = mm01;break;}
      case 0x0f: {mbctype = mbc3;break;}
      case 0x10: {mbctype = mbc3;break;}
      case 0x11: {mbctype = mbc3;break;}
      case 0x12: {mbctype = mbc3;break;}
      case 0x13: {mbctype = mbc3;break;}
      //case 0x15: {mbctype = mbc4;break;}
      //case 0x16: {mbctype = mbc4;break;}
      //case 0x17: {mbctype = mbc4;break;}
      //case 0x19: {mbctype = mbc5;break;}
      //case 0x1a: {mbctype = mbc5;break;}
      //case 0x1b: {mbctype = mbc5;break;}
      //case 0x1c: {mbctype = mbc5;break;}
      //case 0x1d: {mbctype = mbc5;break;}
      //case 0x1e: {mbctype = mbc5;break;}
      //case 0xfe: {mbctype = huc3;break;}
      //case 0xff: {mbctype = huc1;break;}
      default: {throw runtime_error("unknown MBC detected");break;}
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
      default: {throw runtime_error("unknown ROM size detected");break;}
    }
    switch(read_byte(0x0149)) {
      case 0x00: {num_rambanks = 0;break;}
      case 0x01: {num_rambanks = 1;break;} //only the first 2 kBytes are accessible in this case
      case 0x02: {num_rambanks = 1;break;}
      case 0x03: {num_rambanks = 4;break;}
      default: {throw runtime_error("unknown RAM size detected");break;}
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
    cout << e.what() << endl;
    exit(1);
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
    /*
      default case is only used to suppress warnings when compiling
      if mbctype detected above is not one of the ones uncommented here, we throw a runtime_error so we should never get to this point
   */
    default: {break;}
  }
  //increase ROM bank vector as needed
  if (mbctype != romonly && num_rombanks != 0) {
    //first two banks are loaded into 0x0000 - 0x3fff and 0x4000 - 0x7fff
    rombanks.resize(num_rombanks-2);
    try {
      cart.seekg(0x8000,ios::beg);
      for (int i = 0; i < num_rombanks-2; i++) {
        cart.read((char *) &rombanks[i],0x4000);
      }
    }
    catch (const ifstream::failure &e) {
    }
  }
  rombank_ptr = &memory[0x4000];
  cart.close();
  if (num_rambanks != 0) {
    rambanks.resize(num_rambanks-1);
  }
  rambank_ptr = &memory[O_ERAM];
}
//the handle MBC functions need to access and modify the memory model so they should be members of the mem class
//the ROM only handle function should take care of writing to RAM
void mem::handle_romonly(uint16 address, uint8 data) {
  //ignore attempts to write to address < 0x8000
  if (address < 0x8000) {
    return;
  }
  else if (memory[0x0149] == 0x01) {
    if (address < 0xa800) {
      memory[address] = data;
    }
    return;
  }
  else if (memory[0x0149] == 0x02) {
    memory[address] = data;
    return;
  }
};
//address can only be in 0x0000-0x7fff or 0xa000-0xbfff
void mem::handle_mbc1(uint16 address, uint8 data) {
  if (address < 0x2000) {
    ramenabled = (data & 0x0a ? true : false);
    return;
  }
  else if (address < 0x4000) {
    if (num_rombanks > 1) {
      data &= 0x1f;
      if (data == 0x00) {
        data = 0x01;
      }
      switch_rombanks(data | (current_rombank & 0x60));
    }
    return;
  }
  else if (address < 0x6000) {
    if ((mbcmode == rom) && (num_rombanks > 1)) {
      data &= 0x03;
      switch_rombanks(data | (current_rombank & 0x1f));
    }
    else if ((mbcmode == ram) && (num_rambanks > 1)) {
      data &= 0x03;
      switch_rambanks(data);
    }
    return;
  }
  else if (address < 0x8000) {
    mbcmode = (data == 0x01 ? ram : rom);
    return;
  }
  else {
    if (ramenabled && num_rambanks > 0) {
      //if [0x0149] is 8kB or 32 kB we can write anywhere in the external RAM section
      if (memory[0x0149] > 0x01) {
        memory[address] = data;
      }
      //if [0x0149] is 2kB we can only write to the first 0x800 of the external RAM section
      else if ((memory[0x0149] == 0x01) && (address < 0xa800)) {
        memory[address] = data;
      }
    }
    return;
  }
};
void mem::handle_mbc2(uint16 address, uint8 data) {
  if (address < 0x2000) {
    if ((address & 0x0100) == 0x00) {
      ramenabled = (data & 0x0a ? true : false);
    }
    return;
  }
  else if (address < 0x4000) {
    if (address & 0x0100) {
      data &= 0x0f;
      if (data == 0x00) {
        data = 0x01;
      }
      switch_rombanks(data);
    }
    return;
  }
  //MBC2 has built-in RAM bank which consists of 0x200 addresses of 4 bit values
  //since they are 4 bit registers, only the lower 4 bits of the data being written are used
  else if ((address >= 0xa000) && (address < 0xa200)) {
    if (ramenabled) {
      memory[address] = data & 0x0f;
    }
    return;
  }
};
void mem::handle_mbc3(uint16 address, uint8 data) {
  if (address < 0x2000) {
    ramenabled = (data & 0x0a ? true : false);
    return;
  }
  else if (address < 0x4000) {
    if (num_rombanks > 1) {
      data &= 0x7f;
      if (data == 0x00) {
        data = 0x01;
      }
      switch_rombanks(data);
    }
    return;
  }
  else if (address < 0x6000) {
    //if (ramenabled) {
      if (data < 0x04) {
        mbcmode = ram;
        switch_rambanks(data);
      }
      else if (data > 0x07 && data < 0x0d) {
        mbcmode = rtc;
        current_rtc = data - 0x08;
      }
    //}
    return;
  }
  else if (address < 0x8000) {
    return;
  }
  else if (address < 0xc000) {
    if ((mbcmode == ram) && ramenabled) {
      //memory[address] = data;
      *(rambank_ptr + address - O_ERAM) = data;
      return;
    }
    else if (mbcmode == rtc) {
      rtc_registers[current_rtc] = data;
      return;
    }
  }
};

void mem::switch_rombanks(int newbank) {
  if (newbank >= num_rombanks) {
#ifdef DEBUG_ROMBANK
  cout << "failed to switch to ROM bank " << newbank << endl;
  cout << (newbank & 0x60) << " " << (newbank & 0x1f) << endl;
#endif
    return;
  }
#ifdef DEBUG_ROMBANK
  cout << "switching to ROM bank " << newbank << endl;
#endif
  if (newbank == 1) {
    rombank_ptr = &memory[0x4000];
  }
  else {
    rombank_ptr = &rombanks[newbank-2][0];
  }
  current_rombank = newbank;
};

void mem::switch_rambanks(int newbank) {
  if (newbank >= num_rambanks) {
#ifdef DEBUG_ROMBANK
  cout << "failed to switch to ROM bank " << newbank << endl;
#endif
  return;
  }
#ifdef DEBUG_RAMBANK
  cout << "switching to RAM bank " << newbank << endl;
#endif
  if (newbank == 0) {
    rambank_ptr = &memory[O_ERAM];
  }
  else {
    rambank_ptr = &rambanks[newbank-1][0];
  }
  current_rambank = newbank;
};

