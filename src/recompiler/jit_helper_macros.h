//helper macros for building JIT functions
//these should all be one or two liners used to build up the JIT code for each opcode
//FIXME: should DEF_ONE/DEF_TWO be ubyte instead of int?
#define DEF_ONE \
  jit_value one = block->new_constant(1, jit_type_int);

#define DEF_TWO \
  jit_value two = block->new_constant(2, jit_type_int);

#define DEF_0xFF \
  jit_value ff = block->new_constant(0xff, jit_type_ubyte);

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
  reg8_addr = block->new_constant(&r, type_uint8_ptr);

#define GET_REG8_VAL(r) \
  GET_REG8_ADDR(r) \
  jit_value reg8_val = block->insn_load_relative(reg8_addr, 0, jit_type_ubyte);

#define SET_REG8(val) \
  block->insn_store_relative(reg8_addr, 0, val);

#define GET_REG16_ADDR(r) \
  jit_value reg16_addr = block->new_constant(&r, type_uint16_ptr);

#define GET_REG16_VAL(r) \
  GET_REG16_ADDR(r) \
  jit_value reg16_val = block->insn_load_relative(reg16_addr, 0, jit_type_ushort);

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

#define JIT_ARITHMETIC_INIT(r) \
  GET_A_VAL \
  GET_REG8_VAL(r)

#define JIT_ARITHMETIC_INIT_PTR \
  GET_A_VAL \
  GET_REG16_VAL(hl) \
  JIT_READ_BYTE(reg16_val.raw(), reg8_val)

#define JIT_ARITHMETIC_INIT_IMM8 \
  GET_A_VAL \
  GET_BYTE(reg8_val)

//FIXME: don't forget to handle flags here
#define JIT_ADD_FUNC \
  SET_A(a_val + reg8_val)

#define JIT_ADC_FUNC \
  SET_A(a_val + reg8_val)

#define JIT_SUB_FUNC \
  SET_A(a_val - reg8_val)

#define JIT_SBC_FUNC \
  SET_A(a_val - reg8_val)

#define JIT_AND_FUNC \
  SET_A(a_val & reg8_val)

#define JIT_XOR_FUNC \
  SET_A(a_val ^ reg8_val)

#define JIT_OR_FUNC \
  SET_A(a_val | reg8_val)

#define JIT_CP_FUNC
