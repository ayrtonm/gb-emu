#include "jit_helper_macros.h"
#define JIT_PUSH(r) do { \
  GET_SP_VAL \
  GET_REG16_VAL(r) \
  DEF_TWO \
  SET_SP(sp_val - two) \
  JIT_WRITE_WORD(sp_val.raw(), reg_val.raw()) \
} while (0)

#define JIT_POP(r) do { \
  GET_REG16_ADDR(r) \
  GET_SP_VAL \
  DEF_TWO \
  JIT_READ_WORD(sp_val.raw(), temp) \
  SET_SP(sp_val + two) \
  SET_REG(temp) \
} while (0)

//to avoid a naming nightmare let's define loading as getting a value from memory into a register
//and storing as moving a value from a register to memory
//moving a value from register to register is also loading
#define JIT_LOAD_WORD(r) do { \
  GET_WORD \
  GET_REG16_ADDR(r) \
  SET_REG(word) \
} while(0)

#define JIT_LOAD_BYTE(r) do { \
  GET_BYTE \
  GET_REG8_ADDR(r) \
  SET_REG(byte) \
} while(0)

//order of operations matters here since the first two macros define reg_addr
#define JIT_LOAD_REG(dest, src) do { \
  GET_REG8_VAL(src) \
  GET_REG8_ADDR_REDEF(dest) \
  SET_REG(reg_val) \
} while(0)

#define JIT_STORE_A_PTR(r) do { \
  GET_A_VAL \
  GET_REG16_VAL(r) \
  JIT_WRITE_BYTE(reg_val.raw(), a_val.raw()) \
} while (0)

#define JIT_LOAD_A_PTR(r) do { \
  GET_A_ADDR \
  GET_REG16_VAL(r) \
  JIT_READ_WORD(reg_val.raw(), temp) \
  SET_A(temp) \
} while(0)

//FIXME: don't forget to handle flags
#define JIT_INC_REG8(r) do { \
  GET_REG8_VAL(r) \
  DEF_ONE \
  SET_REG(reg_val + one) \
} while(0)

#define JIT_DEC_REG8(r) do { \
  GET_REG8_VAL(r) \
  DEF_ONE \
  SET_REG(reg_val - one) \
} while(0)

//no flag handling here
#define JIT_INC_REG16(r) do { \
  GET_REG16_VAL(r) \
  DEF_ONE \
  SET_REG(reg_val + one) \
} while(0)

#define JIT_DEC_REG16(r) do { \
  GET_REG16_VAL(r) \
  DEF_ONE \
  SET_REG(reg_val - one) \
} while(0)
