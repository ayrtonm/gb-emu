#include "jit_helper_macros.h"
#define JIT_PUSH(r) do { \
  GET_SP_VAL \
  GET_REG16_VAL(r) \
  DEF_TWO \
  SET_SP(sp_val - two) \
  JIT_WRITE_WORD(sp_val.raw(), reg16_val.raw()) \
} while (0)

#define JIT_POP(r) do { \
  GET_REG16_ADDR(r) \
  GET_SP_VAL \
  DEF_TWO \
  JIT_READ_WORD(sp_val.raw(), temp) \
  SET_SP(sp_val + two) \
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
  GET_REG8_ADDR_REDEF(dest) \
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
  GET_A_ADDR \
  GET_WORD(word) \
  JIT_READ_WORD(word.raw(), temp) \
  SET_A(temp) \
} while(0)

#define JIT_STORE_A_IMM16 do { \
  GET_A_VAL \
  GET_WORD(word) \
  JIT_WRITE_WORD(word.raw(), a_val.raw()) \
} while(0)

#define JIT_ADD(r) do { \
  JIT_ARITHMETIC_INIT(r) \
  JIT_ADD_FUNC \
} while(0)

#define JIT_ADD_PTR do { \
  JIT_ARITHMETIC_INIT_PTR \
  JIT_ADD_FUNC \
} while(0)

#define JIT_ADD_IMM8 do { \
  JIT_ARITHMETIC_INIT_IMM8 \
  JIT_ADD_FUNC \
} while(0)

#define JIT_ADC(r) do { \
  JIT_ARITHMETIC_INIT(r) \
  JIT_ADC_FUNC \
} while(0)

#define JIT_ADC_PTR do { \
  JIT_ARITHMETIC_INIT_PTR \
  JIT_ADC_FUNC \
} while(0)

#define JIT_ADC_IMM8 do { \
  JIT_ARITHMETIC_INIT_IMM8 \
  JIT_ADD_FUNC \
} while(0)

#define JIT_SUB(r) do { \
  JIT_ARITHMETIC_INIT(r) \
  JIT_SUB_FUNC \
} while(0)

#define JIT_SUB_PTR do { \
  JIT_ARITHMETIC_INIT_PTR \
  JIT_SUB_FUNC \
} while(0)

#define JIT_SUB_IMM8 do { \
  JIT_ARITHMETIC_INIT_IMM8 \
  JIT_SUB_FUNC \
} while(0)

#define JIT_SBC(r) do { \
  JIT_ARITHMETIC_INIT(r) \
  JIT_SBC_FUNC \
} while(0)

#define JIT_SBC_PTR do { \
  JIT_ARITHMETIC_INIT_PTR \
  JIT_SBC_FUNC \
} while(0)

#define JIT_SBC_IMM8 do { \
  JIT_ARITHMETIC_INIT_IMM8 \
  JIT_SBC_FUNC \
} while(0)

#define JIT_AND(r) do { \
  JIT_ARITHMETIC_INIT(r) \
  JIT_AND_FUNC \
} while(0)

#define JIT_AND_PTR do { \
  JIT_ARITHMETIC_INIT_PTR \
  JIT_AND_FUNC \
} while(0)

#define JIT_AND_IMM8 do { \
  JIT_ARITHMETIC_INIT_IMM8 \
  JIT_AND_FUNC \
} while(0)

#define JIT_XOR(r) do { \
  JIT_ARITHMETIC_INIT(r) \
  JIT_XOR_FUNC \
} while(0)

#define JIT_XOR_PTR do { \
  JIT_ARITHMETIC_INIT_PTR \
  JIT_XOR_FUNC \
} while(0)

#define JIT_XOR_IMM8 do { \
  JIT_ARITHMETIC_INIT_IMM8 \
  JIT_XOR_FUNC \
} while(0)

#define JIT_OR(r) do { \
  JIT_ARITHMETIC_INIT(r) \
  JIT_OR_FUNC \
} while(0)

#define JIT_OR_PTR do { \
  JIT_ARITHMETIC_INIT_PTR \
  JIT_OR_FUNC \
} while(0)

#define JIT_OR_IMM8 do { \
  JIT_ARITHMETIC_INIT_IMM8 \
  JIT_OR_FUNC \
} while(0)

#define JIT_CP(r) do { \
  JIT_ARITHMETIC_INIT(r) \
  JIT_CP_FUNC \
} while(0)

#define JIT_CP_PTR do { \
  JIT_ARITHMETIC_INIT_PTR \
  JIT_CP_FUNC \
} while(0)

#define JIT_CP_IMM8 do { \
  JIT_ARITHMETIC_INIT_IMM8 \
  JIT_CP_FUNC \
} while(0)

//FIXME: remember to handle flags here
#define JIT_CPL do { \
  GET_A_VAL \
  DEF_0xFF \
  SET_A(a_val ^ ff) \
} while(0)

#define JIT_INC_REG8(r) do { \
  GET_REG8_VAL(r) \
  DEF_ONE \
  SET_REG8(reg8_val + one) \
} while(0)

#define JIT_DEC_REG8(r) do { \
  GET_REG8_VAL(r) \
  DEF_ONE \
  SET_REG8(reg8_val - one) \
} while(0)

//no flag handling here
#define JIT_INC_REG16(r) do { \
  GET_REG16_VAL(r) \
  DEF_ONE \
  SET_REG16(reg16_val + one) \
} while(0)

#define JIT_DEC_REG16(r) do { \
  GET_REG16_VAL(r) \
  DEF_ONE \
  SET_REG16(reg16_val - one) \
} while(0)
