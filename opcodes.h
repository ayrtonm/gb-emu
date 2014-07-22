#define INVERT 0
#define NINVERT 1

//finish commented opcodes
case 0x0:  {break;}
case 0x1:  {LD(_BC,IMM16);break;}
case 0x2:  {LD_RM(_BC,_A);break;}
case 0x3:  {_BC++;break;}
case 0x4:  {INC(_B);break;}
case 0x5:  {DEC(_B);break;}
case 0x6:  {LD(_B,IMM8);break;}
case 0x7:  {RLCA;break;}
case 0x8:  {write_word(IMM16,_SP);break;}

case 0x9:  {ADDHL(_BC);break;}
case 0xa:  {LD_MR(_A,_BC);break;}
case 0xb:  {_BC--;break;}
case 0xc:  {INC(_C);break;}
case 0xd:  {DEC(_C);break;}
case 0xe:  {LD(_C,IMM8);break;}
case 0xf:  {RRCA;break;}

//case 0x10:  {break;}//stop
case 0x11:  {LD(_DE,IMM16);break;}
case 0x12:  {LD_RM(_DE,_A);break;}
case 0x13:  {_DE++;break;}
case 0x14:  {INC(_D);break;}
case 0x15:  {DEC(_D);break;}
case 0x16:  {LD(_D,IMM8);break;}
case 0x17:  {RLA;break;}
case 0x18:  {JR(IMM8);break;}

case 0x19:  {ADDHL(_DE);break;}
case 0x1a:  {LD_MR(_A,_DE);break;}
case 0x1b:  {_DE--;break;}
case 0x1c:  {INC(_E);break;}
case 0x1d:  {DEC(_E);break;}
case 0x1e:  {LD(_E,IMM8);break;}
case 0x1f:  {RRA;break;}

case 0x20:  {COND_JR((ZERO==INVERT),IMM8);dt+=1;break;}
case 0x21:  {LD(_HL,IMM16);break;}
case 0x22:  {LD_RM(_HL++,_A);/*_HL++;*/break;}
case 0x23:  {_HL++;break;}
case 0x24:  {INC(_H);break;}
case 0x25:  {DEC(_H);break;}
case 0x26:  {LD(_H,IMM8);break;}
case 0x27:  {DAA;break;}
//07/21/2014
//should be zero == 1
//opcodes work like normal when zero == 0...wtf??
//inverting all(4 + 12) opcodes with conditions
case 0x28:  {COND_JR((ZERO==NINVERT),IMM8);dt+=1;break;}

case 0x29:  {ADDHL(_HL);break;}
case 0x2a:  {LD_MR(_A,_HL++);/*_HL++;*/break;}
case 0x2b:  {_HL--;break;}
case 0x2c:  {INC(_L);break;}
case 0x2d:  {DEC(_L);break;}
case 0x2e:  {LD(_L,IMM8);break;}
case 0x2f:  {CPL;break;}

case 0x30:  {COND_JR((CARRY==INVERT),IMM8);dt+=1;break;}
case 0x31:  {LD(_SP,IMM16);break;}
case 0x32:  {LD_RM(_HL--,_A);/*_HL--;*/break;}
case 0x33:  {_SP++;break;}
case 0x34:  {uint8 temp = READ_BYTE(_HL);INC(temp);write_byte(_HL,temp);break;}
case 0x35:  {uint8 temp = READ_BYTE(_HL);DEC(temp);write_byte(_HL,temp);break;}
case 0x36:  {LD_RM(_HL,IMM8);break;}
case 0x37:  {SCL;break;}
case 0x38:  {COND_JR((CARRY==NINVERT),IMM8);dt+=1;break;}

case 0x39:  {ADDHL(_SP);break;}
case 0x3a:  {LD_MR(_A,_HL--);/*_HL--;*/break;}
case 0x3b:  {_SP--;break;}
case 0x3c:  {INC(_A);break;}
case 0x3d:  {DEC(_A);break;}
case 0x3e:  {LD(_A,IMM8);break;}
case 0x3f:  {CCF;break;}

case 0x40:  {LD(_B,_B);break;}
case 0x41:  {LD(_B,_C);break;}
case 0x42:  {LD(_B,_D);break;}
case 0x43:  {LD(_B,_E);break;}
case 0x44:  {LD(_B,_H);break;}
case 0x45:  {LD(_B,_L);break;}
case 0x46:  {LD_MR(_B,_HL);break;}
case 0x47:  {LD(_B,_A);break;}
case 0x48:  {LD(_C,_B);break;}

case 0x49:  {LD(_C,_C);break;}
case 0x4a:  {LD(_C,_D);break;}
case 0x4b:  {LD(_C,_E);break;}
case 0x4c:  {LD(_C,_H);break;}
case 0x4d:  {LD(_C,_L);break;}
case 0x4e:  {LD_MR(_C,_HL);break;}
case 0x4f:  {LD(_C,_A);break;}

case 0x50:  {LD(_D,_B);break;}
case 0x51:  {LD(_D,_C);break;}
case 0x52:  {LD(_D,_D);break;}
case 0x53:  {LD(_D,_E);break;}
case 0x54:  {LD(_D,_H);break;}
case 0x55:  {LD(_D,_L);break;}
case 0x56:  {LD_MR(_D,_HL);break;}
case 0x57:  {LD(_D,_A);break;}
case 0x58:  {LD(_E,_B);break;}

case 0x59:  {LD(_E,_C);break;}
case 0x5a:  {LD(_E,_D);break;}
case 0x5b:  {LD(_E,_E);break;}
case 0x5c:  {LD(_E,_H);break;}
case 0x5d:  {LD(_E,_L);break;}
case 0x5e:  {LD_MR(_E,_HL);break;}
case 0x5f:  {LD(_E,_A);break;}

case 0x60:  {LD(_H,_B);break;}
case 0x61:  {LD(_H,_C);break;}
case 0x62:  {LD(_H,_D);break;}
case 0x63:  {LD(_H,_E);break;}
case 0x64:  {LD(_H,_H);break;}
case 0x65:  {LD(_H,_L);break;}
case 0x66:  {LD_MR(_H,_HL);break;}
case 0x67:  {LD(_H,_A);break;}
case 0x68:  {LD(_L,_B);break;}

case 0x69:  {LD(_L,_C);break;}
case 0x6a:  {LD(_L,_D);break;}
case 0x6b:  {LD(_L,_E);break;}
case 0x6c:  {LD(_L,_H);break;}
case 0x6d:  {LD(_L,_L);break;}
case 0x6e:  {LD_MR(_L,_HL);break;}
case 0x6f:  {LD(_L,_A);break;}

case 0x70:  {LD_RM(_HL,_B);break;}
case 0x71:  {LD_RM(_HL,_C);break;}
case 0x72:  {LD_RM(_HL,_D);break;}
case 0x73:  {LD_RM(_HL,_E);break;}
case 0x74:  {LD_RM(_HL,_H);break;}
case 0x75:  {LD_RM(_HL,_L);break;}
//I'm still not entirely sure how to implement halt
//considering adding a variable that will be checked before executing opcodes
//that will(should) allow emulate() to continue and keep checking for keypresses
//trying to make halt the same as nop since it should execute the next opcode anyway
//I wonder if the fact that it doesn't stop for the same amount of time would make a difference
//case 0x76:  {break;}//halt
case 0x77:  {LD_RM(_HL,_A);break;}
case 0x78:  {LD(_A,_B);break;}

case 0x79:  {LD(_A,_C);break;}
case 0x7a:  {LD(_A,_D);break;}
case 0x7b:  {LD(_A,_E);break;}
case 0x7c:  {LD(_A,_H);break;}
case 0x7d:  {LD(_A,_L);break;}
case 0x7e:  {LD_MR(_A,_HL);break;}
case 0x7f:  {LD(_A,_A);break;}

case 0x80:  {ADD(_B);break;}
case 0x81:  {ADD(_C);break;}
case 0x82:  {ADD(_D);break;}
case 0x83:  {ADD(_E);break;}
case 0x84:  {ADD(_H);break;}
case 0x85:  {ADD(_L);break;}
case 0x86:  {ADD(READ_BYTE(_HL));break;}
case 0x87:  {ADD(_A);break;}
case 0x88:  {ADC(_B);break;}

case 0x89:  {ADC(_C);break;}
case 0x8a:  {ADC(_D);break;}
case 0x8b:  {ADC(_E);break;}
case 0x8c:  {ADC(_H);break;}
case 0x8d:  {ADC(_L);break;}
case 0x8e:  {ADC(READ_BYTE(_HL));break;}
case 0x8f:  {ADC(_A);break;}

case 0x90:  {SUB(_B);break;}
case 0x91:  {SUB(_C);break;}
case 0x92:  {SUB(_D);break;}
case 0x93:  {SUB(_E);break;}
case 0x94:  {SUB(_H);break;}
case 0x95:  {SUB(_L);break;}
case 0x96:  {SUB(READ_BYTE(_HL));break;}
case 0x97:  {SUB(_A);break;}

case 0x98:  {SBC(_B);break;}
case 0x99:  {SBC(_C);break;}
case 0x9a:  {SBC(_D);break;}
case 0x9b:  {SBC(_E);break;}
case 0x9c:  {SBC(_H);break;}
case 0x9d:  {SBC(_L);break;}
case 0x9e:  {SBC(READ_BYTE(_HL));break;}
case 0x9f:  {SBC(_A);break;}

case 0xa0:  {AND(_B);break;}
case 0xa1:  {AND(_C);break;}
case 0xa2:  {AND(_D);break;}
case 0xa3:  {AND(_E);break;}
case 0xa4:  {AND(_H);break;}
case 0xa5:  {AND(_L);break;}
case 0xa6:  {AND(READ_BYTE(_HL));break;}
case 0xa7:  {AND(_A);break;}

case 0xa8:  {EOR(_B);break;}
case 0xa9:  {EOR(_C);break;}
case 0xaa:  {EOR(_D);break;}
case 0xab:  {EOR(_E);break;}
case 0xac:  {EOR(_H);break;}
case 0xad:  {EOR(_L);break;}
case 0xae:  {EOR(READ_BYTE(_HL));break;}
case 0xaf:  {EOR(_A);break;}

case 0xb0:  {OR(_B);break;}
case 0xb1:  {OR(_C);break;}
case 0xb2:  {OR(_D);break;}
case 0xb3:  {OR(_E);break;}
case 0xb4:  {OR(_H);break;}
case 0xb5:  {OR(_L);break;}
case 0xb6:  {OR(READ_BYTE(_HL));break;}
case 0xb7:  {OR(_A);break;}

case 0xb8:  {CP(_B);break;}
case 0xb9:  {CP(_C);break;}
case 0xba:  {CP(_D);break;}
case 0xbb:  {CP(_E);break;}
case 0xbc:  {CP(_H);break;}
case 0xbd:  {CP(_L);break;}
case 0xbe:  {CP(READ_BYTE(_HL));break;}
case 0xbf:  {CP(_A);break;}

case 0xc0:  {COND_RET((ZERO==INVERT));dt+=3;break;}
case 0xc1:  {POP(_B,_C);break;}
case 0xc2:  {COND_JP((ZERO==INVERT),IMM16);dt+=1;break;}
case 0xc3:  {JP(IMM16);break;}
case 0xc4:  {COND_CALL((ZERO==INVERT),IMM16);dt+=3;break;}
case 0xc5:  {PUSH(_B,_C);break;}
case 0xc6:  {ADD(IMM8);break;}
case 0xc7:  {RST(0x00);break;}
case 0xc8:  {COND_RET((ZERO==NINVERT));dt+=3;break;}

case 0xc9:  {RET;break;}
case 0xca:  {COND_JP((ZERO==NINVERT),IMM16);dt+=1;break;}
case 0xcb:  {break;}//cb opcodes (not used)
case 0xcc:  {COND_CALL((ZERO==NINVERT),IMM16);dt+=3;break;}
case 0xcd:  {CALL(IMM16);break;}
case 0xce:  {ADC(IMM8);break;}
case 0xcf:  {RST(0x08);break;}

case 0xd0:  {COND_RET((CARRY==INVERT));dt+=3;break;}
case 0xd1:  {POP(_D,_E);break;}
case 0xd2:  {COND_JP((CARRY==INVERT),IMM16);dt+=1;break;}
case 0xd3:  {break;}//no opcode
case 0xd4:  {COND_CALL((CARRY==INVERT),IMM16);dt+=3;break;}
case 0xd5:  {PUSH(_D,_E);break;}
case 0xd6:  {SUB(IMM8);break;}
case 0xd7:  {RST(0x10);break;}
case 0xd8:  {COND_RET((CARRY==NINVERT));dt+=3;break;}

case 0xd9:  {RETI;break;}
case 0xda:  {COND_JP((CARRY==NINVERT),IMM16);dt+=1;break;}
case 0xdb:  {break;}//no opcode
case 0xdc:  {COND_CALL((CARRY==NINVERT),IMM16);dt+=3;break;}
case 0xdd:  {break;}//no opcode
case 0xde:  {SBC(IMM8);break;}
case 0xdf:  {RST(0x18);break;}

case 0xe0:  {uint16 temp=IMM8+0xFF00;LD_RM(temp,_A);break;}
case 0xe1:  {POP(_H,_L);break;}
case 0xe2:  {uint16 temp=_C+0xFF00;LD_RM(temp,_A);break;}
case 0xe3:  {break;}//no opcode
case 0xe4:  {break;}//no opcode
case 0xe5:  {PUSH(_H,_L);break;}
case 0xe6:  {AND(IMM8);break;}
case 0xe7:  {RST(0x20);break;}
//case 0xe8:  {break;}//add sp, r8

case 0xe9:  {JP(_HL);break;}
case 0xea:  {LD_RM(IMM16,_A);break;}
case 0xeb:  {break;}//no opcode
case 0xec:  {break;}//no opcode
case 0xed:  {break;}//no opcode
case 0xee:  {EOR(IMM8);break;}
case 0xef:  {RST(0x28);break;}

case 0xf0:  {uint16 temp=IMM8+0xFF00;LD_MR(_A,temp);break;}
case 0xf1:  {POP(_A,_F);break;}
case 0xf2:  {uint16 temp=_C+0xFF00;LD_MR(_A,temp);break;}
case 0xf3:  {_IME=0;break;}
case 0xf4:  {break;}//no opcode
case 0xf5:  {PUSH(_A,_F);break;}
case 0xf6:  {OR(IMM8);break;}
case 0xf7:  {RST(0x30);break;}
//case 0xf8:  {break;}//ld hl, sp+r8

case 0xf9:  {LD(_SP,_HL);break;}
case 0xfa:  {LD_MR(_A,IMM16);break;}
case 0xfb:  {_IME=1;break;}
case 0xfc:  {break;}//no opcode
case 0xfd:  {break;}//no opcode
case 0xfe:  {CP(IMM8);break;}
case 0xff:  {RST(0x38);break;}

default:  {printf("opcode 0x%x not found\n",op);getchar();}
