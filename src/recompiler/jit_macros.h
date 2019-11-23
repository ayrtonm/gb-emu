#include "jit_helper_macros.h"
#define JIT_PUSH(r) do { \
  GET_SP_VAL \
  GET_REG16_VAL(r) \
  sp_val = block->insn_sub(sp_val, two); \
  SET_SP(sp_val) \
  JIT_WRITE_WORD(sp_val.raw(), reg16_val.raw()) \
} while (0)

#define JIT_POP(r) do { \
  GET_REG16_ADDR(r) \
  GET_SP_VAL \
  JIT_READ_WORD(sp_val.raw(), temp) \
  sp_val = block->insn_add(sp_val, two); \
  SET_SP(sp_val) \
  SET_REG16(temp) \
} while (0)

//to avoid a naming nightmare let's define loading as getting a value from memory into a register
//and storing as moving a value from a register to memory
//moving a value from register to register is also loading
#define JIT_LOAD_WORD(r) do { \
  GET_WORD(word) \
  GET_REG16_ADDR(r) \
  SET_REG16(word) \
} while(0)

#define JIT_LOAD_BYTE(r) do { \
  GET_BYTE(byte) \
  GET_REG8_ADDR(r) \
  SET_REG8(byte) \
} while(0)

//order of operations matters here since the first two macros define reg_addr
#define JIT_LOAD_REG(dest, src) do { \
  GET_REG8_VAL(src) \
  GET_REG8_ADDR(dest) \
  SET_REG8(reg8_val) \
} while(0)

#define JIT_STORE_PTR(r, ptr) do { \
  GET_REG8_VAL(r) \
  GET_REG16_VAL(ptr) \
  JIT_WRITE_BYTE(reg16_val.raw(), reg8_val.raw()) \
} while(0)

#define JIT_LOAD_PTR(r, ptr) do { \
  GET_REG8_ADDR(r) \
  GET_REG16_VAL(ptr) \
  JIT_READ_WORD(reg16_val.raw(), temp) \
  SET_REG8(temp) \
} while(0)

#define JIT_STORE_A_PTR(r) JIT_STORE_PTR(af.b.h, r)

#define JIT_LOAD_A_PTR(r) JIT_LOAD_PTR(af.b.h, r)

#define JIT_LOAD_A_IMM16 do { \
  GET_WORD(word) \
  JIT_READ_WORD(word.raw(), temp) \
  SET_A(temp) \
} while(0)

#define JIT_STORE_A_IMM16 do { \
  GET_A_VAL \
  GET_WORD(word) \
  JIT_WRITE_WORD(word.raw(), a_val.raw()) \
} while(0)

#define JIT_ADD(r) JIT_ARITHMETIC(r,ADD)
#define JIT_ADD_PTR JIT_ARITHMETIC_PTR(ADD)
#define JIT_ADD_IMM8 JIT_ARITHMETIC_IMM8(ADD)

#define JIT_ADC(r) JIT_ARITHMETIC(r,ADC)
#define JIT_ADC_PTR JIT_ARITHMETIC_PTR(ADC)
#define JIT_ADC_IMM8 JIT_ARITHMETIC_IMM8(ADC)

#define JIT_SUB(r) JIT_ARITHMETIC(r,SUB)
#define JIT_SUB_PTR JIT_ARITHMETIC_PTR(SUB)
#define JIT_SUB_IMM8 JIT_ARITHMETIC_IMM8(SUB)

#define JIT_SBC(r) JIT_ARITHMETIC(r,SBC)
#define JIT_SBC_PTR JIT_ARITHMETIC_PTR(SBC)
#define JIT_SBC_IMM8 JIT_ARITHMETIC_IMM8(SBC)

#define JIT_AND(r) JIT_ARITHMETIC(r,AND)
#define JIT_AND_PTR JIT_ARITHMETIC_PTR(AND)
#define JIT_AND_IMM8 JIT_ARITHMETIC_IMM8(AND)

#define JIT_XOR(r) JIT_ARITHMETIC(r,XOR)
#define JIT_XOR_PTR JIT_ARITHMETIC_PTR(XOR)
#define JIT_XOR_IMM8 JIT_ARITHMETIC_IMM8(XOR)

#define JIT_OR(r) JIT_ARITHMETIC(r,OR)
#define JIT_OR_PTR JIT_ARITHMETIC_PTR(OR)
#define JIT_OR_IMM8 JIT_ARITHMETIC_IMM8(OR)

#define JIT_CP(r) JIT_ARITHMETIC(r,CP)
#define JIT_CP_PTR JIT_ARITHMETIC_PTR(CP)
#define JIT_CP_IMM8 JIT_ARITHMETIC_IMM8(CP)

#define JIT_CPL do { \
  GET_A_VAL \
  a_val = block->insn_xor(a_val, ff); \
  SET_A(a_val) \
  GET_F_VAL \
  f_val = block->insn_or(f_val, block->insn_or(f_n, f_h)); \
  SET_F(f_val) \
} while(0)

#define JIT_INC_REG8(r) do { \
  GET_REG8_VAL(r) \
  reg8_val = block->insn_add(reg8_val, one); \
  SET_REG8(reg8_val) \
  GET_F_VAL \
  f_val = block->insn_and(f_val, f_c); \
  SET_F(f_val) \
  MODIFY_ZERO_FLAG(reg8_val) \
  MODIFY_H_FLAG(reg8_val) \
} while(0)

#define JIT_DEC_REG8(r) do { \
  GET_REG8_VAL(r) \
  reg8_val = block->insn_sub(reg8_val, one); \
  SET_REG8(reg8_val) \
  GET_F_VAL \
  f_val = block->insn_and(f_val, f_c); \
  f_val = block->insn_or(f_val, f_n); \
  SET_F(f_val) \
  MODIFY_ZERO_FLAG(reg8_val) \
  MODIFY_H_FLAG(reg8_val) \
} while(0)

#define JIT_INC_REG16(r) do { \
  GET_REG16_VAL(r) \
  reg16_val = block->insn_add(reg16_val, one); \
  SET_REG16(reg16_val) \
} while(0)

#define JIT_DEC_REG16(r) do { \
  GET_REG16_VAL(r) \
  reg16_val = block->insn_sub(reg16_val, one); \
  SET_REG16(reg16_val) \
} while(0)
