case 0x00: {
  jit_insn_nop(function);
  break;
case 0x01: {
  //read a word from memory then load it into the bc register
  //jit_value mem_word_addr = block.new_constant(&m.read_word, 
  jit_value read_word = block.
  jit_value bc_addr = block.new_constant(&bc, type_uint16_ptr);
  block.insn_store_relative(bc_addr, 0,);
  break;
}
default: {
  jit_insn_nop(function);
  break;
}
