case 0x00: {
  break;
}
case 0x01: {
  //read a 16-bit argument (address) then read from memory at that address (word) and load that value into the bc register (bc_addr)
  jit_value address = block.new_constant(block.get_last_word(), jit_type_ushort);
  jit_value_t args[] = {m_addr.raw(), address.raw()};
  jit_value word = block.insn_call_native(NULL, (void *)&mem::read_word, read_word_signature, args, 2, 0);
  jit_value bc_addr = block.new_constant(&bc, type_uint16_ptr);
  block.insn_store_relative(bc_addr, 0, word);
  break;
}
case 0xc5: {
  //push bc register onto the stack
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
