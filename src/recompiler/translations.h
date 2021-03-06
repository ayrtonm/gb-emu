case 0x00: {break;} // for some reason LibJIT's insn_nop() isn't available when using C++ so nop should be handled as an unconditional jump
case 0x01: {JIT_LOAD_WORD(bc);break;}
case 0x02: {JIT_STORE_A_PTR(bc);break;}
case 0x03: {JIT_INC_REG16(bc);break;}
case 0x04: {JIT_INC_REG8(bc.b.h);break;}
case 0x05: {JIT_DEC_REG8(bc.b.h);break;}
case 0x06: {JIT_LOAD_BYTE(bc.b.h);break;}
//case 0x07: {break;}

//case 0x08: {break;}
//case 0x09: {break;}
case 0x0a: {JIT_LOAD_A_PTR(bc);break;}
//case 0x0b: {break;}
case 0x0c: {JIT_INC_REG8(bc.b.l);break;}
case 0x0d: {JIT_DEC_REG8(bc.b.l);break;}
case 0x0e: {JIT_LOAD_BYTE(bc.b.l);break;}
//case 0x0f: {break;}

//case 0x10: {break;}
case 0x11: {JIT_LOAD_WORD(de);break;}
case 0x12: {JIT_STORE_A_PTR(de);break;}
case 0x13: {JIT_INC_REG16(de);break;}
case 0x14: {JIT_INC_REG8(de.b.h);break;}
case 0x15: {JIT_DEC_REG8(de.b.h);break;}
case 0x16: {JIT_LOAD_BYTE(de.b.h);break;}
//case 0x17: {break;}

//case 0x18: {break;}
//case 0x19: {break;}
case 0x1a: {JIT_LOAD_A_PTR(de);break;}
//case 0x1b: {break;}
case 0x1c: {JIT_INC_REG8(de.b.l);break;}
case 0x1d: {JIT_DEC_REG8(de.b.l);break;}
case 0x1e: {JIT_LOAD_BYTE(de.b.l);break;}
//case 0x1f: {break;}

//case 0x20: {break;}
case 0x21: {JIT_LOAD_WORD(hl);break;}
case 0x22: {JIT_STORE_A_PTR(hl);JIT_INC_REG16(hl);break;}
case 0x23: {JIT_INC_REG16(hl);break;}
case 0x24: {JIT_INC_REG8(hl.b.h);break;}
case 0x25: {JIT_DEC_REG8(hl.b.h);break;}
case 0x26: {JIT_LOAD_BYTE(hl.b.h);break;}
//case 0x27: {break;}

//case 0x28: {break;}
//case 0x29: {break;}
case 0x2a: {JIT_LOAD_A_PTR(hl);JIT_INC_REG16(hl);break;}
//case 0x2b: {break;}
case 0x2c: {JIT_INC_REG8(hl.b.l);break;}
case 0x2d: {JIT_DEC_REG8(hl.b.l);break;}
case 0x2e: {JIT_LOAD_BYTE(hl.b.l);break;}
case 0x2f: {JIT_CPL;break;}

//case 0x30: {break;}
case 0x31: {JIT_LOAD_WORD(sp);break;}
case 0x32: {JIT_STORE_A_PTR(hl);JIT_DEC_REG16(hl);break;}
case 0x33: {JIT_INC_REG16(sp);break;}
//case 0x34: {break;}
//case 0x35: {break;}
//case 0x36: {break;}
//case 0x37: {break;}

//case 0x38: {break;}
//case 0x39: {break;}
case 0x3a: {JIT_LOAD_A_PTR(hl);JIT_DEC_REG16(hl);break;}
//case 0x3b: {break;}
case 0x3c: {JIT_INC_REG8(af.b.h);break;}
case 0x3d: {JIT_DEC_REG8(af.b.h);break;}
case 0x3e: {JIT_LOAD_BYTE(af.b.h);break;}
//case 0x3f: {break;}

case 0x40: {JIT_LOAD_REG(bc.b.h, bc.b.h);break;}
case 0x41: {JIT_LOAD_REG(bc.b.h, bc.b.l);break;}
case 0x42: {JIT_LOAD_REG(bc.b.h, de.b.h);break;}
case 0x43: {JIT_LOAD_REG(bc.b.h, de.b.l);break;}
case 0x44: {JIT_LOAD_REG(bc.b.h, hl.b.h);break;}
case 0x45: {JIT_LOAD_REG(bc.b.h, hl.b.l);break;}
case 0x46: {JIT_LOAD_PTR(bc.b.h, hl);break;}
case 0x47: {JIT_LOAD_REG(bc.b.h, af.b.h);break;}

case 0x48: {JIT_LOAD_REG(bc.b.l, bc.b.h);break;}
case 0x49: {JIT_LOAD_REG(bc.b.l, bc.b.l);break;}
case 0x4a: {JIT_LOAD_REG(bc.b.l, de.b.h);break;}
case 0x4b: {JIT_LOAD_REG(bc.b.l, de.b.l);break;}
case 0x4c: {JIT_LOAD_REG(bc.b.l, hl.b.h);break;}
case 0x4d: {JIT_LOAD_REG(bc.b.l, hl.b.l);break;}
case 0x4e: {JIT_LOAD_PTR(bc.b.l, hl);break;}
case 0x4f: {JIT_LOAD_REG(bc.b.l, af.b.h);break;}

case 0x50: {JIT_LOAD_REG(de.b.h, bc.b.h);break;}
case 0x51: {JIT_LOAD_REG(de.b.h, bc.b.l);break;}
case 0x52: {JIT_LOAD_REG(de.b.h, de.b.h);break;}
case 0x53: {JIT_LOAD_REG(de.b.h, de.b.l);break;}
case 0x54: {JIT_LOAD_REG(de.b.h, hl.b.h);break;}
case 0x55: {JIT_LOAD_REG(de.b.h, hl.b.l);break;}
case 0x56: {JIT_LOAD_PTR(de.b.h, hl);break;}
case 0x57: {JIT_LOAD_REG(de.b.h, af.b.h);break;}

case 0x58: {JIT_LOAD_REG(de.b.l, bc.b.h);break;}
case 0x59: {JIT_LOAD_REG(de.b.l, bc.b.l);break;}
case 0x5a: {JIT_LOAD_REG(de.b.l, de.b.h);break;}
case 0x5b: {JIT_LOAD_REG(de.b.l, de.b.l);break;}
case 0x5c: {JIT_LOAD_REG(de.b.l, hl.b.h);break;}
case 0x5d: {JIT_LOAD_REG(de.b.l, hl.b.l);break;}
case 0x5e: {JIT_LOAD_PTR(de.b.l, hl);break;}
case 0x5f: {JIT_LOAD_REG(de.b.l, af.b.h);break;}

case 0x60: {JIT_LOAD_REG(hl.b.h, bc.b.h);break;}
case 0x61: {JIT_LOAD_REG(hl.b.h, bc.b.l);break;}
case 0x62: {JIT_LOAD_REG(hl.b.h, de.b.h);break;}
case 0x63: {JIT_LOAD_REG(hl.b.h, de.b.l);break;}
case 0x64: {JIT_LOAD_REG(hl.b.h, hl.b.h);break;}
case 0x65: {JIT_LOAD_REG(hl.b.h, hl.b.l);break;}
case 0x66: {JIT_LOAD_PTR(hl.b.h, hl);break;}
case 0x67: {JIT_LOAD_REG(hl.b.h, af.b.h);break;}

case 0x68: {JIT_LOAD_REG(hl.b.l, bc.b.h);break;}
case 0x69: {JIT_LOAD_REG(hl.b.l, bc.b.l);break;}
case 0x6a: {JIT_LOAD_REG(hl.b.l, de.b.h);break;}
case 0x6b: {JIT_LOAD_REG(hl.b.l, de.b.l);break;}
case 0x6c: {JIT_LOAD_REG(hl.b.l, hl.b.h);break;}
case 0x6d: {JIT_LOAD_REG(hl.b.l, hl.b.l);break;}
case 0x6e: {JIT_LOAD_PTR(hl.b.l, hl);break;}
case 0x6f: {JIT_LOAD_REG(hl.b.l, af.b.h);break;}

case 0x70: {JIT_STORE_PTR(bc.b.h, hl);break;}
case 0x71: {JIT_STORE_PTR(bc.b.l, hl);break;}
case 0x72: {JIT_STORE_PTR(de.b.h, hl);break;}
case 0x73: {JIT_STORE_PTR(de.b.l, hl);break;}
case 0x74: {JIT_STORE_PTR(hl.b.h, hl);break;}
case 0x75: {JIT_STORE_PTR(hl.b.l, hl);break;}
//case 0x76: {break;}
case 0x77: {JIT_STORE_PTR(af.b.h, hl);break;}

case 0x78: {JIT_LOAD_REG(af.b.h, bc.b.h);break;}
case 0x79: {JIT_LOAD_REG(af.b.h, bc.b.l);break;}
case 0x7a: {JIT_LOAD_REG(af.b.h, de.b.h);break;}
case 0x7b: {JIT_LOAD_REG(af.b.h, de.b.l);break;}
case 0x7c: {JIT_LOAD_REG(af.b.h, hl.b.h);break;}
case 0x7d: {JIT_LOAD_REG(af.b.h, hl.b.l);break;}
case 0x7e: {JIT_LOAD_PTR(af.b.h, hl);break;}
case 0x7f: {JIT_LOAD_REG(af.b.h, af.b.h);break;}

case 0x80: {JIT_ADD(bc.b.h);break;}
case 0x81: {JIT_ADD(bc.b.l);break;}
case 0x82: {JIT_ADD(de.b.h);break;}
case 0x83: {JIT_ADD(de.b.l);break;}
case 0x84: {JIT_ADD(hl.b.h);break;}
case 0x85: {JIT_ADD(hl.b.l);break;}
case 0x86: {JIT_ADD_PTR;break;}
case 0x87: {JIT_ADD(af.b.h);break;}

case 0x88: {JIT_ADC(bc.b.h);break;}
case 0x89: {JIT_ADC(bc.b.l);break;}
case 0x8a: {JIT_ADC(de.b.h);break;}
case 0x8b: {JIT_ADC(de.b.l);break;}
case 0x8c: {JIT_ADC(hl.b.h);break;}
case 0x8d: {JIT_ADC(hl.b.l);break;}
case 0x8e: {JIT_ADC_PTR;break;}
case 0x8f: {JIT_ADC(af.b.h);break;}

case 0x90: {JIT_SUB(bc.b.h);break;}
case 0x91: {JIT_SUB(bc.b.l);break;}
case 0x92: {JIT_SUB(de.b.h);break;}
case 0x93: {JIT_SUB(de.b.l);break;}
case 0x94: {JIT_SUB(hl.b.h);break;}
case 0x95: {JIT_SUB(hl.b.l);break;}
case 0x96: {JIT_SUB_PTR;break;}
case 0x97: {JIT_SUB(af.b.h);break;}

case 0x98: {JIT_SBC(bc.b.h);break;}
case 0x99: {JIT_SBC(bc.b.l);break;}
case 0x9a: {JIT_SBC(de.b.h);break;}
case 0x9b: {JIT_SBC(de.b.l);break;}
case 0x9c: {JIT_SBC(hl.b.h);break;}
case 0x9d: {JIT_SBC(hl.b.l);break;}
case 0x9e: {JIT_SBC_PTR;break;}
case 0x9f: {JIT_SBC(af.b.h);break;}

case 0xa0: {JIT_AND(bc.b.h);break;}
case 0xa1: {JIT_AND(bc.b.l);break;}
case 0xa2: {JIT_AND(de.b.h);break;}
case 0xa3: {JIT_AND(de.b.l);break;}
case 0xa4: {JIT_AND(hl.b.h);break;}
case 0xa5: {JIT_AND(hl.b.l);break;}
case 0xa6: {JIT_AND_PTR;break;}
case 0xa7: {JIT_AND(af.b.h);break;}

case 0xa8: {JIT_XOR(bc.b.h);break;}
case 0xa9: {JIT_XOR(bc.b.l);break;}
case 0xaa: {JIT_XOR(de.b.h);break;}
case 0xab: {JIT_XOR(de.b.l);break;}
case 0xac: {JIT_XOR(hl.b.h);break;}
case 0xad: {JIT_XOR(hl.b.l);break;}
case 0xae: {JIT_XOR_PTR;break;}
case 0xaf: {JIT_XOR(af.b.h);break;}

case 0xb0: {JIT_OR(bc.b.h);break;}
case 0xb1: {JIT_OR(bc.b.l);break;}
case 0xb2: {JIT_OR(de.b.h);break;}
case 0xb3: {JIT_OR(de.b.l);break;}
case 0xb4: {JIT_OR(hl.b.h);break;}
case 0xb5: {JIT_OR(hl.b.l);break;}
case 0xb6: {JIT_OR_PTR;break;}
case 0xb7: {JIT_OR(af.b.h);break;}

case 0xb8: {JIT_CP(bc.b.h);break;}
case 0xb9: {JIT_CP(bc.b.l);break;}
case 0xba: {JIT_CP(de.b.h);break;}
case 0xbb: {JIT_CP(de.b.l);break;}
case 0xbc: {JIT_CP(hl.b.h);break;}
case 0xbd: {JIT_CP(hl.b.l);break;}
case 0xbe: {JIT_CP_PTR;break;}
case 0xbf: {JIT_CP(af.b.h);break;}

case 0xc1: {JIT_POP(bc);break;}
case 0xc5: {JIT_PUSH(bc);break;}
case 0xc6: {JIT_ADD_IMM8;break;}

case 0xd1: {JIT_POP(de);break;}
case 0xd5: {JIT_PUSH(de);break;}
case 0xd6: {JIT_SUB_IMM8;break;}

case 0xe1: {JIT_POP(hl);break;}
case 0xe5: {JIT_PUSH(hl);break;}
case 0xe6: {JIT_AND_IMM8;break;}

case 0xe0: {JIT_STORE_A_HI;break;}

case 0xea: {JIT_STORE_A_IMM16;break;}

case 0xf0: {JIT_LOAD_A_HI;break;}

case 0xfa: {JIT_LOAD_A_IMM16;break;}
case 0xfe: {JIT_CP_IMM8;break;}
