case 0x00: {
  break;
}
case 0x01: {
  //read a 16-bit argument (address) then read from memory at that address (word) and load that value into the bc register (bc_addr)
  jit_value_t address = block.new_constant(block.get_last_word(), jit_type_ushort).raw();
  jit_value_t args[] = {address};
  jit_value word = block.insn_call_native(NULL, (void *)&mem::read_word, read_word_signature, args, 1, 0);
  jit_value bc_addr = block.new_constant(&bc, type_uint16_ptr);
  block.insn_store_relative(bc_addr, 0, word);
  break;
}
default: {
  break;
}
