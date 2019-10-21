case 0x00: {
  break;
}
case 0x01: {
  //load a word into the bc register
  //ld $bc xx
  jit_value word = block.new_constant(block.get_last_word(), jit_type_ushort);
  jit_value bc_addr = block.new_constant(&bc, type_uint16_ptr);
  block.insn_store_relative(bc_addr, 0, word);
  break;
}
case 0x11: {
  break;
}
case 0x21: {
  break;
}
case 0x22: {
  break;
}
case 0x25: {
  break;
}
case 0x2a: {
  break;
}
case 0x2d: {
  break;
}
case 0x2e: {
  break;
}
case 0x2f: {
  break;
}
case 0x3e: {
  break;
}
case 0x47: {
  break;
}
case 0x4f: {
  break;
}
case 0x5: {
  break;
}
case 0x56: {
  break;
}
case 0x5f: {
  break;
}
case 0x6: {
  break;
}
case 0x72: {
  break;
}
case 0x78: {
  break;
}
case 0x7b: {
  break;
}
case 0x80: {
  break;
}
case 0x90: {
  break;
}
case 0x97: {
  break;
}
case 0xb8: {
  break;
}
case 0xc1: {
  //pop the bc register from the stack
  //pop $bc
  jit_value bc_addr = block.new_constant(&bc, type_uint16_ptr);
  jit_value sp_addr = block.new_constant(&sp, type_uint16_ptr);
  jit_value increment = block.new_constant(2, jit_type_int);

  jit_value sp_reg = block.insn_load_relative(sp_addr, 0, jit_type_ushort);
  jit_value_t args[] = {m_addr.raw(), sp_reg.raw()};
  jit_value temp1 = block.insn_call_native(NULL, (void *)&mem::read_word, read_word_signature, args, 2, 0);
  jit_value temp2 = sp_reg + increment;
  block.insn_store_relative(bc_addr, 0, temp1);
  block.insn_store_relative(sp_addr, 0, temp2);
  break;
}
case 0xc5: {
  //push the bc register onto the stack
  //push $bc
  jit_value bc_addr = block.new_constant(&bc, type_uint16_ptr);
  jit_value sp_addr = block.new_constant(&sp, type_uint16_ptr);
  jit_value bc_reg = block.insn_load_relative(bc_addr, 0, jit_type_ushort);
  jit_value sp_reg = block.insn_load_relative(sp_addr, 0, jit_type_ushort);
  jit_value decrement = block.new_constant(2, jit_type_int);
  sp_reg = sp_reg - decrement;
  block.insn_store_relative(sp_addr, 0, sp_reg);
  jit_value_t args[] = {m_addr.raw(), sp_reg.raw(), bc_reg.raw()};
  block.insn_call_native(NULL, (void *)&mem::write_word, write_word_signature, args, 3, 0);
  break;
}
case 0xc6: {
  break;
}
case 0xd1: {
  break;
}
case 0xd5: {
  break;
}
case 0xd6: {
  break;
}
case 0xe1: {
  break;
}
case 0xe5: {
  break;
}
case 0xe6: {
  break;
}
case 0xea: {
  break;
}
case 0xfa: {
  break;
}
case 0xfe: {
  break;
}
