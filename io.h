case IO_LCDSTAT: {
  //lower 3 bits of LCD STAT are read only
  memory[O_IO+IO_LCDSTAT] = (memory[O_IO+IO_LCDSTAT] & 0x07) | (data & 0xf8);
  break;
}
case IO_BGP: {
  memory[address] = data;
  update_palette(2, data);
  break;
}
case IO_OBP0: {
  memory[address] = data;
  update_palette(0, data);
  break;
}
case IO_OBP1: {
  memory[address] = data;
  update_palette(1, data);
  break;
}
case IO_LY: {
  //this is read only, attempting to write to it "resets the counter"
  memory[O_IO+IO_LY] = 0;
  break;
}
case IO_DIV: {
  memory[O_IO+IO_DIV] = 0x00;
  divtimer = 0;
  break;
}
case IO_TAC: {
  tacthreshold = tacvals[data & 0x03]; 
  timatimer = 0;
  memory[O_IO+IO_TAC] = data;
  break;
}
case IO_JOYP: {
  uint8 keysselected = data & 0x30;
  //if special keys are selected
  if (keysselected == JOYP_DIRECTION_SELECTED) {
    loadeddirection = false;
    memory[O_IO+IO_JOYP] = (joyspecial & 0x0f)|(data & 0xf0);
  }
  //if direction keys are selected
  else if (keysselected == JOYP_SPECIAL_SELECTED) {
    loadeddirection = true;
    memory[O_IO+IO_JOYP] = (joydirection & 0x0f)|(data & 0xf0);
  }
  //if neither set is selected
  else if (keysselected == (JOYP_DIRECTION_SELECTED|JOYP_SPECIAL_SELECTED)) {
    //keep previous setting for loadeddirection
    memory[O_IO+IO_JOYP] = (data & 0xf0);
  }
  //if both sets are selected (undefined behavior)
  else if (keysselected == 0x00) {
    //ignore attempt to write to memory
    memory[O_IO+IO_JOYP] = data;
  }
  break;
}
case IO_DMA: {
  if (data <= 0xF1) {
    dmatimer = 160;
    dmatransfering = true;
    //all data is written immediately after the DMA is executed which very likely does not match the hardware's behavior
    //this should not matter since accessable memory is limited during DMA transfer
    for (int i = 0; i < 160; i++) {
      memory[0xFE00 + i] = memory[(data << 8) + i];
    }
  }
  break;
}
//writing to sound registers is only allowed if sound is enabled in NR52
//this restriction doesn't apply to NR52
//only implementing appropriate read/write permissions and side effects for now (i.e. reading/writing won't actually produce sound)
case IO_NR10: {
  if (memory[O_IO + IO_NR52] & 0x80) {
    memory[address] = data;
  }
  break;
}
//bits 6-7 are read/write, other bits are write only and produce side effects
case IO_NR11: {
  if (memory[O_IO + IO_NR52] & 0x80) {
    //since lower 6 bits are write only, store the data somewhere else so read_byte() will return 0 for these bits
    ch1t = data & 0x3f;
    memory[address] = data & 0xc0;
  }
  break;
}
//read/write with side effects
case IO_NR12: {
  if (memory[O_IO + IO_NR52] & 0x80) {
    memory[address] = data;
  }
  break;
}
//write only with side effects
case IO_NR13: {
  if (memory[O_IO + IO_NR52] & 0x80) {
    memory[address] = 0;
  }
  break;
}
//can only read from bit 6, bits 0-2 and bit 7 are write only with side effects
case IO_NR14: {
  if (memory[O_IO + IO_NR52] & 0x80) {
    //if writing to bit 7, set flag in NR52
    if (data & 0x80) {
      memory[O_IO+IO_NR52] |= 0x01;
      ch1timer = (64 - ch1t)*3906;
    }
    memory[address] = data & 0x40;
  }
  break;
}
//bits 6-7 are read/write, bits 0-5 are write only with side effects
case IO_NR21: {
  if (memory[O_IO + IO_NR52] & 0x80) {
    //since lower 6 bits are write only, store the data somewhere else so read_byte() will return 0 for these bits
    ch2t = data & 0x3f;
    memory[address] = data & 0xc0;
  }
  break;
}
//read/write with side effects
case IO_NR22: {
  if (memory[O_IO + IO_NR52] & 0x80) {
    memory[address] = data;
  }
  break;
}
//write only with side effects
case IO_NR23: {
  if (memory[O_IO + IO_NR52] & 0x80) {
    memory[address] = 0;
  }
  break;
}
//can only read from bit 6, bits 0-2 and bit 7 are write only with side effects
case IO_NR24: {
  if (memory[O_IO + IO_NR52] & 0x80) {
    //if writing to bit 7, set flag in NR52
    if (data & 0x80) {
      memory[O_IO+IO_NR52] |= 0x02;
      ch2timer = (64 - ch2t)*3906;
    }
    memory[address] = data & 0x40;
  }
  break;
}
//bit 7 is read/write with side effects
case IO_NR30: {
  if (memory[O_IO + IO_NR52] & 0x80) {
    memory[address] = data & 0x80;
  }
  break;
}
//permissions not specified in pandocs
//possibly a timer/counter
case IO_NR31: {
  if (memory[O_IO + IO_NR52] & 0x80) {
    //since lower 6 bits are write only, store the data somewhere else so read_byte() will return 0 for these bits
    ch3t = data;
    //pandocs doesn't specify behavior of writing to NR31 so I'm following same pattern in NR11 and NR21
    //memory[address] = data;
  }
  break;
}
//bits 5-6 are read/write with side effects
case IO_NR32: {
  if (memory[O_IO + IO_NR52] & 0x80) {
    memory[address] = data & 0x60;
  }
  break;
}
//write only with side effects
case IO_NR33: {
  if (memory[O_IO + IO_NR52] & 0x80) {
    memory[address] = 0;
  }
  break;
}
//can only read from bit 6, bits 0-2 and bit 7 are write only with side effects
case IO_NR34: {
  if (memory[O_IO + IO_NR52] & 0x80) {
    //if writing to bit 7, set flag in NR52
    if (data & 0x80) {
      memory[O_IO+IO_NR52] |= 0x04;
      ch3timer = (256 - ch3t)*3906;
    }
    memory[address] = data & 0x40;
  }
  break;
}
//bits 0-5 are read/write with side effects
case IO_NR41: {
  if (memory[O_IO + IO_NR52] & 0x80) {
    //since lower 6 bits are write only, store the data somewhere else so read_byte() will return 0 for these bits
    ch4t = data & 0x3f;
    //pandocs doesn't specify behavior of writing to NR31 so I'm following same pattern in NR11 and NR21
    memory[address] = data & 0xc0;
  }
  break;
}
//read/write with side effects
case IO_NR42: {
  if (memory[O_IO + IO_NR52] & 0x80) {
    memory[address] = data;
  }
  break;
}
//read/write with side effects
case IO_NR43: {
  if (memory[O_IO + IO_NR52] & 0x80) {
    memory[address] = data;
  }
  break;
}
//bit 6 is read/write, bit 7 is write only with side effects
case IO_NR44: {
  if (memory[O_IO + IO_NR52] & 0x80) {
    //if writing to bit 7, set flag in NR52
    if (data & 0x80) {
      ch4timer = (64 - ch4t)*3906;
      memory[O_IO+IO_NR52] |= 0x08;
    }
    memory[address] = data & 0x40;
  }
  break;
}
//read/write with side effects
case IO_NR50: {
  if (memory[O_IO + IO_NR52] & 0x80) {
    memory[address] = data;
  }
  break;
}
//read/write with side effects
case IO_NR51: {
  if (memory[O_IO + IO_NR52] & 0x80) {
    memory[address] = data;
  }
  break;
}
//bit 7 is read/write, bits 0-3 are read only
case IO_NR52: {
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
  break;
}
