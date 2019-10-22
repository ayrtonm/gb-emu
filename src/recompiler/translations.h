case 0x00: {break;}
case 0x01: {JIT_LOAD_WORD(bc);break;}
case 0x02: {JIT_STORE_A_PTR(bc);break;}
case 0x03: {JIT_INC_REG16(bc);break;}
case 0x04: {JIT_INC_REG8(bc.b.h);break;}
case 0x05: {JIT_DEC_REG8(bc.b.h);break;}
case 0x06: {JIT_LOAD_BYTE(bc.b.h);break;}
case 0x07: {break;}

case 0x08: {break;}
case 0x09: {break;}
case 0x0a: {JIT_LOAD_A_PTR(bc);break;}
case 0x0b: {break;}
case 0x0c: {JIT_INC_REG8(bc.b.l);break;}
case 0x0d: {JIT_DEC_REG8(bc.b.l);break;}
case 0x0e: {JIT_LOAD_BYTE(bc.b.l);break;}
case 0x0f: {break;}

case 0x10: {break;}
case 0x11: {JIT_LOAD_WORD(de);break;}
case 0x12: {JIT_STORE_A_PTR(de);break;}
case 0x13: {JIT_INC_REG16(de);break;}
case 0x14: {JIT_INC_REG8(de.b.h);break;}
case 0x15: {JIT_DEC_REG8(de.b.h);break;}
case 0x16: {JIT_LOAD_BYTE(de.b.h);break;}
case 0x17: {break;}

case 0x18: {break;}
case 0x19: {break;}
case 0x1a: {JIT_LOAD_A_PTR(de);break;}
case 0x1b: {break;}
case 0x1c: {JIT_INC_REG8(de.b.l);break;}
case 0x1d: {JIT_DEC_REG8(de.b.l);break;}
case 0x1e: {JIT_LOAD_BYTE(de.b.l);break;}
case 0x1f: {break;}

case 0x20: {break;}
case 0x21: {JIT_LOAD_WORD(hl);break;}
case 0x22: {JIT_STORE_A_PTR(hl);JIT_INC_REG16(hl);break;}
case 0x23: {JIT_INC_REG16(hl);break;}
case 0x24: {JIT_INC_REG8(hl.b.h);break;}
case 0x25: {JIT_DEC_REG8(hl.b.l);break;}
case 0x26: {JIT_LOAD_BYTE(hl.b.h);break;}
case 0x27: {break;}

case 0x28: {break;}
case 0x29: {break;}
case 0x2a: {JIT_LOAD_A_PTR(hl);JIT_INC_REG16(hl);break;}
case 0x2b: {break;}
case 0x2c: {JIT_INC_REG8(hl.b.l);break;}
case 0x2d: {JIT_DEC_REG8(hl.b.l);break;}
case 0x2e: {JIT_LOAD_BYTE(hl.b.l);break;}
case 0x2f: {
  break;
}

case 0x30: {break;}
case 0x31: {JIT_LOAD_WORD(sp);break;}
case 0x32: {JIT_STORE_A_PTR(hl);JIT_DEC_REG16(hl);break;}
case 0x33: {JIT_INC_REG16(sp);break;}
case 0x34: {break;}
case 0x35: {break;}
case 0x36: {break;}
case 0x37: {break;}

case 0x38: {break;}
case 0x39: {break;}
case 0x3a: {JIT_LOAD_A_PTR(hl);JIT_DEC_REG16(hl);break;}
case 0x3b: {break;}
case 0x3c: {JIT_INC_REG8(af.b.h);break;}
case 0x3d: {JIT_DEC_REG8(af.b.h);break;}
case 0x3e: {JIT_LOAD_BYTE(af.b.h);break;}
case 0x3f: {break;}

case 0x40: {JIT_LOAD_REG(bc.b.h, bc.b.h);break;}
case 0x41: {JIT_LOAD_REG(bc.b.h, bc.b.l;break;}
case 0x42: {JIT_LOAD_REG(bc.b.h, de.b.h;break;}
case 0x43: {JIT_LOAD_REG(bc.b.h, de.b.l;break;}
case 0x44: {JIT_LOAD_REG(bc.b.h, hl.b.h;break;}
case 0x45: {JIT_LOAD_REG(bc.b.h, hl.b.l;break;}
case 0x46: {break;}
case 0x47: {JIT_LOAD_REG(bc.b.h, af.b.h;break;}

case 0x48: {JIT_LOAD_REG(bc.b.l, bc.b.h);break;}
case 0x49: {JIT_LOAD_REG(bc.b.l, bc.b.l;break;}
case 0x4a: {JIT_LOAD_REG(bc.b.l, de.b.h;break;}
case 0x4b: {JIT_LOAD_REG(bc.b.l, de.b.l;break;}
case 0x4c: {JIT_LOAD_REG(bc.b.l, hl.b.h;break;}
case 0x4d: {JIT_LOAD_REG(bc.b.l, hl.b.l;break;}
case 0x4e: {break;}
case 0x4f: {JIT_LOAD_REG(bc.b.l, af.b.h;break;}

case 0x50: {JIT_LOAD_REG(de.b.h, bc.b.h);break;}
case 0x51: {JIT_LOAD_REG(de.b.h, bc.b.l;break;}
case 0x52: {JIT_LOAD_REG(de.b.h, de.b.h;break;}
case 0x53: {JIT_LOAD_REG(de.b.h, de.b.l;break;}
case 0x54: {JIT_LOAD_REG(de.b.h, hl.b.h;break;}
case 0x55: {JIT_LOAD_REG(de.b.h, hl.b.l;break;}
case 0x56: {
  break;
}
case 0x57: {JIT_LOAD_REG(de.b.h, af.b.h;break;}

case 0x58: {JIT_LOAD_REG(de.b.l, bc.b.h);break;}
case 0x59: {JIT_LOAD_REG(de.b.l, bc.b.l;break;}
case 0x5a: {JIT_LOAD_REG(de.b.l, de.b.h;break;}
case 0x5b: {JIT_LOAD_REG(de.b.l, de.b.l;break;}
case 0x5c: {JIT_LOAD_REG(de.b.l, hl.b.h;break;}
case 0x5d: {JIT_LOAD_REG(de.b.l, hl.b.l;break;}
case 0x5e: {break;}
case 0x5f: {JIT_LOAD_REG(de.b.l, af.b.h;break;}

case 0x60: {JIT_LOAD_REG(hl.b.h, bc.b.h);break;}
case 0x61: {JIT_LOAD_REG(hl.b.h, bc.b.l;break;}
case 0x62: {JIT_LOAD_REG(hl.b.h, de.b.h;break;}
case 0x63: {JIT_LOAD_REG(hl.b.h, de.b.l;break;}
case 0x64: {JIT_LOAD_REG(hl.b.h, hl.b.h;break;}
case 0x65: {JIT_LOAD_REG(hl.b.h, hl.b.l;break;}
case 0x66: {break;}
case 0x67: {JIT_LOAD_REG(hl.b.h, af.b.h;break;}

case 0x68: {JIT_LOAD_REG(hl.b.l, bc.b.h);break;}
case 0x69: {JIT_LOAD_REG(hl.b.l, bc.b.l;break;}
case 0x6a: {JIT_LOAD_REG(hl.b.l, de.b.h;break;}
case 0x6b: {JIT_LOAD_REG(hl.b.l, de.b.l;break;}
case 0x6c: {JIT_LOAD_REG(hl.b.l, hl.b.h;break;}
case 0x6d: {JIT_LOAD_REG(hl.b.l, hl.b.l;break;}
case 0x6e: {break;}
case 0x6f: {JIT_LOAD_REG(hl.b.l, af.b.h;break;}

case 0x70: {break;}
case 0x71: {break;}
case 0x72: {
  break;
}
case 0x73: {break;}
case 0x74: {break;}
case 0x75: {break;}
case 0x76: {break;}
case 0x77: {break;}

case 0x78: {JIT_LOAD_REG(af.b.h, bc.b.h);break;}
case 0x79: {JIT_LOAD_REG(af.b.h, bc.b.l;break;}
case 0x7a: {JIT_LOAD_REG(af.b.h, de.b.h;break;}
case 0x7b: {JIT_LOAD_REG(af.b.h, de.b.l;break;}
case 0x7c: {JIT_LOAD_REG(af.b.h, hl.b.h;break;}
case 0x7d: {JIT_LOAD_REG(af.b.h, hl.b.l;break;}
case 0x7e: {break;}
case 0x7f: {JIT_LOAD_REG(af.b.h, af.b.h;break;}

case 0x80: {
  break;
}
case 0x90: {
  break;
}
case 0x97: {
  break;
}
case 0xb8: {
  break;
}
//pop $bc
case 0xc1: {JIT_POP(bc);break;}
//push $bc
case 0xc5: {JIT_PUSH(bc);break;}
case 0xc6: {
  break;
}
case 0xd1: {
  break;
}
case 0xd5: {
  break;
}
case 0xd6: {
  break;
}
case 0xe1: {
  break;
}
case 0xe5: {
  break;
}
case 0xe6: {
  break;
}
case 0xea: {
  break;
}
case 0xfa: {
  break;
}
case 0xfe: {
  break;
}
