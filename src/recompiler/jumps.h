//JR
case 0x18: {
  address += (signed char)m.read_byte(address + 1);
  break;
}
//JP
case 0xc3: {
  address = m.read_word(address + 1);
  break;
}
//RST 0x00
case 0xc7: {
  //push the program counter onto the stack then jump to 0x00
  sp.w -= 2;
  m.write_word(sp.w, address);
  address = 0x00;
  break;
}
//RET
case 0xc9: {
  //pop the program counter from the stack
  address = m.read_word(sp.w);
  sp.w += 2;
  break;
}
//CALL
case 0xcd: {
  //push the program counter onto the stack
  sp.w -= 2;
  m.write_word(sp.w, address);
  address = m.read_word(address + 1);
  break;
}
//RST 0x08
case 0xcf: {
  //push the program counter onto the stack
  sp.w -= 2;
  m.write_word(sp.w, address);
  address = 0x08;
  break;
}
//RST 0x10
case 0xd7: {
  //push the program counter onto the stack
  sp.w -= 2;
  m.write_word(sp.w, address);
  address = 0x10;
  break;
}
//RETI
case 0xd9: {
  //set IME to 1
  //pop the program counter from the stack
  ime = 1;
  address = m.read_word(sp.w);
  sp.w += 2;
  break;
}
//RST 0x18
case 0xdf: {
  //push the program counter onto the stack
  sp.w -= 2;
  m.write_word(sp.w, address);
  address = 0x18;
  break;
}
//RST 0x20
case 0xe7: {
  //push the program counter onto the stack
  sp.w -= 2;
  m.write_word(sp.w, address);
  address = 0x20;
  break;
}
//JP
case 0xe9: {
  //get address from hl registers
  address = hl.w;
}
//RST 0x28
case 0xef: {
  //push the program counter onto the stack
  sp.w -= 2;
  m.write_word(sp.w, address);
  address = 0x28;
  break;
}
//RST 0x30
case 0xf7: {
  //push the program counter onto the stack
  sp.w -= 2;
  m.write_word(sp.w, address);
  address = 0x30;
  break;
}
//RST 0x38
case 0xff: {
  //push the program counter onto the stack
  sp.w -= 2;
  m.write_word(sp.w, address);
  address = 0x38;
  break;
}
