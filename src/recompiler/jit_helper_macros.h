//helper macros for building JIT functions
//these should all be one or two liners used to build up the JIT code for each opcode
#define DEF_ONE \
  jit_value one = block->new_constant(1, jit_type_int);

#define DEF_TWO \
  jit_value two = block->new_constant(2, jit_type_int);

#define GET_A_ADDR \
  jit_value a_addr = block->new_constant(&af.b.h, type_uint8_ptr);

#define GET_A_VAL \
  GET_A_ADDR \
  jit_value a_val = block->insn_load_relative(a_addr, 0, jit_type_ubyte);

#define SET_A(val) \
  block->insn_store_relative(a_addr, 0, val);

#define GET_SP_ADDR \
  jit_value sp_addr = block->new_constant(&sp, type_uint16_ptr);

#define GET_SP_VAL \
  GET_SP_ADDR \
  jit_value sp_val = block->insn_load_relative(sp_addr, 0, jit_type_ushort);

#define SET_SP(val) \
  block->insn_store_relative(sp_addr, 0, val);

#define GET_REG8_ADDR(r) \
  jit_value GET_REG8_ADDR_REDEF(r)

//this redefines reg_addr, use with caution
#define GET_REG8_ADDR_REDEF(r) \
  reg_addr = block->new_constant(&r, type_uint8_ptr);

#define GET_REG8_VAL(r) \
  GET_REG8_ADDR(r) \
  jit_value reg_val = block->insn_load_relative(reg_addr, 0, jit_type_ubyte);

#define GET_REG16_ADDR(r) \
  jit_value reg_addr = block->new_constant(&r, type_uint16_ptr);

#define GET_REG16_VAL(r) \
  GET_REG16_ADDR(r) \
  jit_value reg_val = block->insn_load_relative(reg_addr, 0, jit_type_ushort);

#define SET_REG(val) \
  block->insn_store_relative(reg_addr, 0, val);

#define GET_BYTE \
  jit_value byte = block->new_constant(block->get_last_byte(), jit_type_ushort);

#define GET_WORD \
  jit_value word = block->new_constant(block->get_last_word(), jit_type_ushort);

#define SET_WRITE_ARGS(addr, val) \
  jit_value_t args[] = {m_addr.raw(), addr, val};

#define SET_READ_ARGS(addr) \
  jit_value_t args[] = {m_addr.raw(), addr};

#define JIT_WRITE_BYTE(addr, val) \
  SET_WRITE_ARGS(addr, val) \
  block->insn_call_native(NULL, (void *)&mem::write_byte, write_byte_signature, args, 3, 0);

#define JIT_WRITE_WORD(addr, val) \
  SET_WRITE_ARGS(addr, val) \
  block->insn_call_native(NULL, (void *)&mem::write_word, write_word_signature, args, 3, 0);

#define JIT_READ_BYTE(addr, val) \
  SET_READ_ARGS(addr) \
  jit_value res = block->insn_call_native(NULL, (void *)&mem::read_byte, read_byte_signature, args, 2, 0);

#define JIT_READ_WORD(addr, res) \
  SET_READ_ARGS(addr) \
  jit_value res = block->insn_call_native(NULL, (void *)&mem::read_word, read_word_signature, args, 2, 0);

