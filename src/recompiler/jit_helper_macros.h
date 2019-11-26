//helper macros for building JIT functions
//these should all be one or two liners used to build up the JIT code for each opcode
#define GET_F_VAL \
  f_val = block->insn_load_relative(f_addr, 0, jit_type_ubyte);

#define GET_A_VAL \
  a_val = block->insn_load_relative(a_addr, 0, jit_type_ubyte);

#define GET_SP_VAL \
  sp_val = block->insn_load_relative(sp_addr, 0, jit_type_ushort);

#define SET_F(val) \
  val = block->insn_convert(val, jit_type_ubyte); \
  block->insn_store_relative(f_addr, 0, val.raw());

#define SET_A(val) \
  val = block->insn_convert(val, jit_type_ubyte); \
  block->insn_store_relative(a_addr, 0, val);

#define SET_SP(val) \
  val = block->insn_convert(val, jit_type_ushort); \
  block->insn_store_relative(sp_addr, 0, val);

#define GET_REG8_ADDR(r) \
  reg8_addr = block->new_constant(&r, type_uint8_t_ptr);

#define GET_REG8_VAL(r) \
  GET_REG8_ADDR(r) \
  reg8_val = block->insn_load_relative(reg8_addr, 0, jit_type_ubyte);

#define SET_REG8(val) \
  val = block->insn_convert(val, jit_type_ubyte); \
  block->insn_store_relative(reg8_addr, 0, val);

#define GET_REG16_ADDR(r) \
  reg16_addr = block->new_constant(&r, type_uint16_t_ptr);

#define GET_REG16_VAL(r) \
  GET_REG16_ADDR(r) \
  reg16_val = block->insn_load_relative(reg16_addr, 0, jit_type_ushort);

#define SET_REG16(val) \
  val = block->insn_convert(val, jit_type_ushort); \
  block->insn_store_relative(reg16_addr, 0, val);

#define GET_BYTE(byte) \
  jit_value byte = block->new_constant(block->get_last_byte(), jit_type_ubyte);

#define GET_WORD(word) \
  jit_value word = block->new_constant(block->get_last_word(), jit_type_ushort);

#define SET_WRITE_ARGS(addr, val) \
  jit_value_t args[] = {m_addr.raw(), addr, val};

#define SET_READ_ARGS(addr) \
  jit_value_t args[] = {m_addr.raw(), addr};

#define JIT_INVALIDATE_BLOCKS(addr) \
  jit_value_t invalidate_args[] = {addr}; \
  /*block->insn_call_native(NULL, (void *)&cache::invalidate_blocks, invalidate_blocks_signature, invalidate_args, 3, 0);*/

#define JIT_WRITE_BYTE(addr, val) \
  JIT_INVALIDATE_BLOCKS(addr) \
  SET_WRITE_ARGS(addr, val) \
  block->insn_call_native(NULL, (void *)&mem::write_byte, write_byte_signature, args, 3, 0);

#define JIT_WRITE_WORD(addr, val) \
  JIT_INVALIDATE_BLOCKS(addr) \
  SET_WRITE_ARGS(addr, val) \
  block->insn_call_native(NULL, (void *)&mem::write_word, write_word_signature, args, 3, 0);

#define JIT_READ_BYTE(addr, res) \
  SET_READ_ARGS(addr) \
  jit_value res = block->insn_call_native(NULL, (void *)&mem::read_byte, read_byte_signature, args, 2, 0);

#define JIT_READ_WORD(addr, res) \
  SET_READ_ARGS(addr) \
  jit_value res = block->insn_call_native(NULL, (void *)&mem::read_word, read_word_signature, args, 2, 0);

#define JIT_ARITHMETIC(r,op) do { \
  GET_A_VAL \
  GET_REG8_VAL(r) \
  JIT_##op##_FUNC \
} while(0)

#define JIT_ARITHMETIC_PTR(op) do { \
  GET_A_VAL \
  GET_REG16_VAL(hl) \
  JIT_READ_BYTE(reg16_val.raw(), reg8_val) \
  JIT_##op##_FUNC \
} while(0)

#define JIT_ARITHMETIC_IMM8(op) do { \
  GET_A_VAL \
  GET_BYTE(reg8_val) \
  JIT_##op##_FUNC \
} while(0)

#define JIT_ADD_FUNC \
  a_val = block->insn_add(a_val, reg8_val); \
  SET_A(a_val) \
  SET_F(zero) \
  MODIFY_ZERO_FLAG(a_val) \
  MODIFY_CARRY_FLAG(a_val) \
  MODIFY_H_FLAG(a_val)

#define JIT_ADC_FUNC \
  a_val = block->insn_add(a_val, reg8_val); \
  GET_F_VAL \
  temp = block->insn_and(f_val, f_c); \
  temp = block->insn_shr(temp, four); \
  a_val = block->insn_add(a_val, temp); \
  SET_A(a_val) \
  SET_F(zero) \
  MODIFY_ZERO_FLAG(a_val) \
  MODIFY_CARRY_FLAG(a_val) \
  MODIFY_H_FLAG(a_val)

#define JIT_SUB_FUNC \
  a_val = block->insn_sub(a_val, reg8_val); \
  SET_A(a_val) \
  SET_F(f_n) \
  MODIFY_ZERO_FLAG(a_val) \
  MODIFY_CARRY_FLAG(a_val) \
  MODIFY_H_FLAG(a_val)

#define JIT_SBC_FUNC \
  a_val = block->insn_sub(a_val, reg8_val); \
  GET_F_VAL \
  temp = block->insn_and(f_val, f_c); \
  temp = block->insn_shr(temp, four); \
  a_val = block->insn_sub(a_val, temp); \
  SET_A(a_val) \
  SET_F(f_n) \
  MODIFY_ZERO_FLAG(a_val) \
  MODIFY_CARRY_FLAG(a_val) \
  MODIFY_H_FLAG(a_val)

#define JIT_AND_FUNC \
  a_val = block->insn_and(a_val, reg8_val); \
  SET_A(a_val) \
  SET_F(f_h) \
  MODIFY_ZERO_FLAG(a_val)

#define JIT_XOR_FUNC \
  a_val = block->insn_xor(a_val, reg8_val); \
  SET_A(a_val) \
  SET_F(zero) \
  MODIFY_ZERO_FLAG(a_val)

#define JIT_OR_FUNC \
  a_val = block->insn_or(a_val, reg8_val); \
  SET_A(a_val) \
  SET_F(zero) \
  MODIFY_ZERO_FLAG(a_val)

#define JIT_CP_FUNC \
  jit_value temp = block->insn_sub(a_val, reg8_val); \
  SET_F(f_n) \
  MODIFY_ZERO_FLAG(temp) \
  MODIFY_CARRY_FLAG(temp) \
  MODIFY_H_FLAG(temp)

/*
  Make sure to set `condition` before using this macro
  first, explicitly clear the nth bit in f_val
    f_val &= ((1 << n) ^ 0xFF)
  then bitwise OR it with the nth bit's new value
    flag_val = (1 << n)
    f_val |= flag_val
*/
#define MODIFY_FLAG_BODY(flag) \
  GET_F_VAL \
  not_flag = block->insn_xor(flag, ff); \
  f_val = block->insn_and(f_val, not_flag); \
  flag_val = block->insn_mul(condition, flag); \
  f_val = block->insn_or(f_val, flag_val); \
  SET_F(f_val)

#define MODIFY_ZERO_FLAG(temp) \
  condition = block->insn_eq(temp, zero); \
  MODIFY_FLAG_BODY(f_z)

//this is only the simplest way to check if we need to set the carry flag
//for some rotate/shift opcodes we have to check by testing one of the lower 8 bits
#define MODIFY_CARRY_FLAG(temp) \
  condition = block->insn_gt(temp, ff); \
  MODIFY_FLAG_BODY(f_c)

#define MODIFY_H_FLAG(temp) \
  /*condition = block->insn_X(temp, Y);*/ \
  /*MODIFY_FLAG_BODY(f_h)*/

#define JIT_CB(r,op) do { \
  GET_REG8_VAL(r) \
  JIT_##op##_FUNC \
} while(0)

#define JIT_CB_PTR(op) do { \
  GET_REG16_VAL(hl) \
  JIT_READ_BYTE(reg16_val.raw(), reg8_val) \
  JIT_##op##_FUNC \
} while(0)

#define JIT_CB_BIT(r,op,n) do { \
  GET_REG8_VAL(r) \
  temp = block->new_constant(n, jit_type_ushort); \
  JIT_##op##_FUNC(n) \
} while(0)

#define JIT_CB_BIT_PTR(op,n) do { \
  GET_REG16_VAL(hl) \
  JIT_READ_BYTE(reg16_val.raw(), reg8_val) \
  temp = block->new_constant(n, jit_type_ushort); \
  JIT_##op##_FUNC(n) \
} while(0)

#define JIT_SET_FUNC(n) \
  reg8_val = block->insn_or(reg8_val, temp); \
  SET_REG8(reg8_val)

#define JIT_RESET_FUNC(n) \
  temp = block->insn_xor(temp, ff); \
  reg8_val = block->insn_and(reg8_val, temp); \
  SET_REG8(reg8_val)

#define JIT_TEST_FUNC(n) \
  reg8_val = block->insn_and(reg8_val, temp); \
  GET_F_VAL \
  f_val = block->insn_and(f_val, f_c); \
  f_val = block->insn_or(f_val, f_h); \
  SET_F(f_val) \
  MODIFY_ZERO_FLAG(reg8_val)

#define JIT_SWAP_FUNC \
  temp = block->insn_or(reg8_val, upper_bits); \
  temp = block->insn_shr(temp, four); \
  reg8_val = block->insn_or(reg8_val, lower_bits); \
  reg8_val = block->insn_shl(reg8_val, four); \
  reg8_val = block->insn_or(reg8_val, temp); \
  SET_REG8(reg8_val) \
  SET_F(zero) \
  MODIFY_ZERO_FLAG(reg8_val)

#define JIT_RLC_FUNC \
  temp = block->insn_shl(reg8_val, one); \
  reg8_val = block->insn_shr(reg8_val, seven); \
  reg8_val = block->insn_and(reg8_val, one); \
  condition = block->insn_eq(reg8_val, one); \
  MODIFY_FLAG_BODY(f_c) \
  reg8_val = block->insn_or(reg8_val, temp); \
  SET_REG8(reg8_val) \
  MODIFY_ZERO_FLAG(reg8_val)

#define JIT_RRC_FUNC

/*
  first let temp = r << 1
  then r >>= 7
  condition = r & 1
  load f_val
  f_val &= f_c
  f_val >>= 4
  at this point f_val contains 0 or 1 depending on the value of the carry flag
  note that we will not be storing this f_val since the flags are messed up
  in other words, we need to reload f_val before modifying the flags
  r = temp | f_val
  store r
  modify carry flag based on condition (f_val is reloaded here)
  modify zero flag based on r
*/
#define JIT_RL_FUNC \
  temp = block->insn_shl(reg8_val, one); \
  reg8_val = block->insn_shr(reg8_val, seven); \
  condition = block->insn_eq(reg8_val, one); \
  GET_F_VAL \
  f_val = block->insn_and(f_val, f_c); \
  f_val = block->insn_shr(f_val, four); \
  reg8_val = block->insn_or(temp, f_val); \
  SET_REG8(reg8_val) \
  MODIFY_FLAG_BODY(f_c) \
  MODIFY_ZERO_FLAG(reg8_val)

#define JIT_RR_FUNC
#define JIT_SLA_FUNC
#define JIT_SRA_FUNC
#define JIT_SRL_FUNC
