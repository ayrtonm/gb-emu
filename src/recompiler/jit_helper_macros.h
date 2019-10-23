//helper macros for building JIT functions
//these should all be one or two liners used to build up the JIT code for each opcode
#define GET_F_VAL \
  f_val = block->insn_load_relative(f_addr, 0, jit_type_ubyte);

#define GET_A_VAL \
  a_val = block->insn_load_relative(a_addr, 0, jit_type_ubyte);

#define GET_SP_VAL \
  sp_val = block->insn_load_relative(sp_addr, 0, jit_type_ushort);

#define SET_F(val) \
  block->insn_store_relative(f_addr, 0, val);

#define SET_A(val) \
  block->insn_store_relative(a_addr, 0, val);

#define SET_SP(val) \
  block->insn_store_relative(sp_addr, 0, val);

#define GET_REG8_ADDR(r) \
  reg8_addr = block->new_constant(&r, type_uint8_ptr);

#define GET_REG8_VAL(r) \
  GET_REG8_ADDR(r) \
  reg8_val = block->insn_load_relative(reg8_addr, 0, jit_type_ubyte);

#define SET_REG8(val) \
  block->insn_store_relative(reg8_addr, 0, val);

#define GET_REG16_ADDR(r) \
  reg16_addr = block->new_constant(&r, type_uint16_ptr);

#define GET_REG16_VAL(r) \
  GET_REG16_ADDR(r) \
  reg16_val = block->insn_load_relative(reg16_addr, 0, jit_type_ushort);

#define SET_REG16(val) \
  block->insn_store_relative(reg16_addr, 0, val);

#define GET_BYTE(byte) \
  jit_value byte = block->new_constant(block->get_last_byte(), jit_type_ushort);

#define GET_WORD(word) \
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

#define JIT_ADD_FUNC \
  a_val = block->insn_add(a_val, reg8_val); \
  SET_A(a_val) \
  SET_F(zero) \
  SET_F_Z(a_val) \
  SET_F_C(a_val) \
  SET_F_H(a_val)

//FIXME: add the carry
#define JIT_ADC_FUNC \
  a_val = block->insn_add(a_val, reg8_val); \
  SET_A(a_val) \
  SET_F(zero) \
  SET_F_Z(a_val) \
  SET_F_C(a_val) \
  SET_F_H(a_val)

#define JIT_SUB_FUNC \
  a_val = block->insn_sub(a_val, reg8_val); \
  SET_A(a_val) \
  SET_F(f_n) \
  SET_F_Z(a_val) \
  SET_F_C(a_val) \
  SET_F_H(a_val)

//FIXME: add the carry
#define JIT_SBC_FUNC \
  a_val = block->insn_sub(a_val, reg8_val); \
  SET_A(a_val) \
  SET_F(f_n) \
  SET_F_Z(a_val) \
  SET_F_C(a_val) \
  SET_F_H(a_val)

#define JIT_AND_FUNC \
  a_val = block->insn_and(a_val, reg8_val); \
  SET_A(a_val) \
  SET_F(f_h) \
  SET_F_Z(a_val)

#define JIT_XOR_FUNC \
  a_val = block->insn_xor(a_val, reg8_val); \
  SET_A(a_val) \
  SET_F(zero) \
  SET_F_Z(a_val)

#define JIT_OR_FUNC \
  a_val = block_insn_or(a_val, reg8_val); \
  SET_A(a_val) \
  SET_F(zero) \
  SET_F_Z(a_val)

#define JIT_CP_FUNC \
  jit_value temp = block->insn_sub(a_val, reg8_val); \
  SET_F(f_n) \
  SET_F_Z(temp) \
  SET_F_C(temp) \
  SET_F_H(temp)

#define SET_F_FLAG(flag) \
  skip = block->new_label(); \
  block->insn_branch_if_not(take_branch, skip); \
  GET_F_VAL \
  f_val = block->insn_or(f_val, flag); \
  SET_F(f_val) \
  block->insn_label(skip);

#define SET_F_Z(temp) \
  take_branch = block->insn_eq(temp, zero); \
  SET_F_FLAG(f_z)

#define SET_F_C(temp) \
  take_branch = block->insn_gt(temp, ff); \
  SET_F_FLAG(f_c)

#define SET_F_H(temp)
  //take_branch = block->insn() \
  //SET_F_FLAG(f_h)
