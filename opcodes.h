//finish commented opcodes
/*************************************************************************************************
maybe problem with emulator is in opcode macros being correct but numbers being associated with the wrong operations
*************************************************************************************************/
case 0x0:  {break;}
case 0x1:  {LD(bc.w,m.read_word(pc.w-2));break;}
case 0x2:  {LD_RM(bc.w,af.b.h);break;}
case 0x3:  {bc.w++;break;}
case 0x4:  {INC(bc.b.h);break;}
case 0x5:  {DEC(bc.b.h);break;}
case 0x6:  {LD(bc.b.h,m.read_byte(pc.w-1));break;}
case 0x7:  {RLCA;break;}

case 0x8:  {m.write_word(m.read_word(pc.w-2),sp.w);break;}
case 0x9:  {ADDHL(bc.w);break;}
case 0xa:  {LD_MR(af.b.h,bc.w);break;}
case 0xb:  {bc.w--;break;}
case 0xc:  {INC(bc.b.l);break;}
case 0xd:  {DEC(bc.b.l);break;}
case 0xe:  {LD(bc.b.l,m.read_byte(pc.w-1));break;}
case 0xf:  {RRCA;break;}

case 0x10:  {halt = 1;break;}//stop
case 0x11:  {LD(de.w,m.read_word(pc.w-2));break;}
case 0x12:  {LD_RM(de.w,af.b.h);break;}
case 0x13:  {de.w++;break;}
case 0x14:  {INC(de.b.h);break;}
case 0x15:  {DEC(de.b.h);break;}
case 0x16:  {LD(de.b.h,m.read_byte(pc.w-1));break;}
case 0x17:  {RLA;break;}

case 0x18:  {JR(m.read_byte(pc.w-1));break;}
case 0x19:  {ADDHL(de.w);break;}
case 0x1a:  {LD_MR(af.b.h,de.w);break;}
case 0x1b:  {de.w--;break;}
case 0x1c:  {INC(de.b.l);break;}
case 0x1d:  {DEC(de.b.l);break;}
case 0x1e:  {LD(de.b.l,m.read_byte(pc.w-1));break;}
case 0x1f:  {RRA;break;}

case 0x20:  {COND_JR(!(af.b.l & F_Z),m.read_byte(pc.w-1));break;}
case 0x21:  {LD(hl.w,m.read_word(pc.w-2));break;}
case 0x22:  {LD_RM(hl.w++,af.b.h);/*hl.w++;*/break;}
case 0x23:  {hl.w++;break;}
case 0x24:  {INC(hl.b.h);break;}
case 0x25:  {DEC(hl.b.h);break;}
case 0x26:  {LD(hl.b.h,m.read_byte(pc.w-1));break;}
case 0x27:  {DAA;break;}

case 0x28:  {COND_JR((af.b.l & F_Z),m.read_byte(pc.w-1));break;}
case 0x29:  {ADDHL(hl.w);break;}
case 0x2a:  {LD_MR(af.b.h,hl.w++);/*hl.w++;*/break;}
case 0x2b:  {hl.w--;break;}
case 0x2c:  {INC(hl.b.l);break;}
case 0x2d:  {DEC(hl.b.l);break;}
case 0x2e:  {LD(hl.b.l,m.read_byte(pc.w-1));break;}
case 0x2f:  {CPL;break;}

case 0x30:  {COND_JR(!(af.b.l & F_C),m.read_byte(pc.w-1));break;}
case 0x31:  {LD(sp.w,m.read_word(pc.w-2));break;}
case 0x32:  {LD_RM(hl.w--,af.b.h);/*hl.w--;*/break;}
case 0x33:  {sp.w++;break;}
case 0x34:  {uint8 temp = m.read_byte(hl.w);INC(temp);m.write_byte(hl.w,temp);break;}
case 0x35:  {uint8 temp = m.read_byte(hl.w);DEC(temp);m.write_byte(hl.w,temp);break;}
case 0x36:  {LD_RM(hl.w,m.read_byte(pc.w-1));break;}
case 0x37:  {SCL;break;}

case 0x38:  {COND_JR((af.b.l & F_C),m.read_byte(pc.w-1));break;}
case 0x39:  {ADDHL(sp.w);break;}
case 0x3a:  {LD_MR(af.b.h,hl.w--);/*hl.w--;*/break;}
case 0x3b:  {sp.w--;break;}
case 0x3c:  {INC(af.b.h);break;}
case 0x3d:  {DEC(af.b.h);break;}
case 0x3e:  {LD(af.b.h,m.read_byte(pc.w-1));break;}
case 0x3f:  {CCF;break;}

case 0x40:  {LD(bc.b.h,bc.b.h);break;}
case 0x41:  {LD(bc.b.h,bc.b.l);break;}
case 0x42:  {LD(bc.b.h,de.b.h);break;}
case 0x43:  {LD(bc.b.h,de.b.l);break;}
case 0x44:  {LD(bc.b.h,hl.b.h);break;}
case 0x45:  {LD(bc.b.h,hl.b.l);break;}
case 0x46:  {LD_MR(bc.b.h,hl.w);break;}
case 0x47:  {LD(bc.b.h,af.b.h);break;}

case 0x48:  {LD(bc.b.l,bc.b.h);break;}
case 0x49:  {LD(bc.b.l,bc.b.l);break;}
case 0x4a:  {LD(bc.b.l,de.b.h);break;}
case 0x4b:  {LD(bc.b.l,de.b.l);break;}
case 0x4c:  {LD(bc.b.l,hl.b.h);break;}
case 0x4d:  {LD(bc.b.l,hl.b.l);break;}
case 0x4e:  {LD_MR(bc.b.l,hl.w);break;}
case 0x4f:  {LD(bc.b.l,af.b.h);break;}

case 0x50:  {LD(de.b.h,bc.b.h);break;}
case 0x51:  {LD(de.b.h,bc.b.l);break;}
case 0x52:  {LD(de.b.h,de.b.h);break;}
case 0x53:  {LD(de.b.h,de.b.l);break;}
case 0x54:  {LD(de.b.h,hl.b.h);break;}
case 0x55:  {LD(de.b.h,hl.b.l);break;}
case 0x56:  {LD_MR(de.b.h,hl.w);break;}
case 0x57:  {LD(de.b.h,af.b.h);break;}

case 0x58:  {LD(de.b.l,bc.b.h);break;}
case 0x59:  {LD(de.b.l,bc.b.l);break;}
case 0x5a:  {LD(de.b.l,de.b.h);break;}
case 0x5b:  {LD(de.b.l,de.b.l);break;}
case 0x5c:  {LD(de.b.l,hl.b.h);break;}
case 0x5d:  {LD(de.b.l,hl.b.l);break;}
case 0x5e:  {LD_MR(de.b.l,hl.w);break;}
case 0x5f:  {LD(de.b.l,af.b.h);break;}

case 0x60:  {LD(hl.b.h,bc.b.h);break;}
case 0x61:  {LD(hl.b.h,bc.b.l);break;}
case 0x62:  {LD(hl.b.h,de.b.h);break;}
case 0x63:  {LD(hl.b.h,de.b.l);break;}
case 0x64:  {LD(hl.b.h,hl.b.h);break;}
case 0x65:  {LD(hl.b.h,hl.b.l);break;}
case 0x66:  {LD_MR(hl.b.h,hl.w);break;}
case 0x67:  {LD(hl.b.h,af.b.h);break;}

case 0x68:  {LD(hl.b.l,bc.b.h);break;}
case 0x69:  {LD(hl.b.l,bc.b.l);break;}
case 0x6a:  {LD(hl.b.l,de.b.h);break;}
case 0x6b:  {LD(hl.b.l,de.b.l);break;}
case 0x6c:  {LD(hl.b.l,hl.b.h);break;}
case 0x6d:  {LD(hl.b.l,hl.b.l);break;}
case 0x6e:  {LD_MR(hl.b.l,hl.w);break;}
case 0x6f:  {LD(hl.b.l,af.b.h);break;}

case 0x70:  {LD_RM(hl.w,bc.b.h);break;}
case 0x71:  {LD_RM(hl.w,bc.b.l);break;}
case 0x72:  {LD_RM(hl.w,de.b.h);break;}
case 0x73:  {LD_RM(hl.w,de.b.l);break;}
case 0x74:  {LD_RM(hl.w,hl.b.h);break;}
case 0x75:  {LD_RM(hl.w,hl.b.l);break;}
case 0x76:  {halt = 1;break;}
case 0x77:  {LD_RM(hl.w,af.b.h);break;}

case 0x78:  {LD(af.b.h,bc.b.h);break;}
case 0x79:  {LD(af.b.h,bc.b.l);break;}
case 0x7a:  {LD(af.b.h,de.b.h);break;}
case 0x7b:  {LD(af.b.h,de.b.l);break;}
case 0x7c:  {LD(af.b.h,hl.b.h);break;}
case 0x7d:  {LD(af.b.h,hl.b.l);break;}
case 0x7e:  {LD_MR(af.b.h,hl.w);break;}
case 0x7f:  {LD(af.b.h,af.b.h);break;}

case 0x80:  {ADD(bc.b.h);break;}
case 0x81:  {ADD(bc.b.l);break;}
case 0x82:  {ADD(de.b.h);break;}
case 0x83:  {ADD(de.b.l);break;}
case 0x84:  {ADD(hl.b.h);break;}
case 0x85:  {ADD(hl.b.l);break;}
case 0x86:  {ADD(m.read_byte(hl.w));break;}
case 0x87:  {ADD(af.b.h);break;}

case 0x88:  {ADC(bc.b.h);break;}
case 0x89:  {ADC(bc.b.l);break;}
case 0x8a:  {ADC(de.b.h);break;}
case 0x8b:  {ADC(de.b.l);break;}
case 0x8c:  {ADC(hl.b.h);break;}
case 0x8d:  {ADC(hl.b.l);break;}
case 0x8e:  {ADC(m.read_byte(hl.w));break;}
case 0x8f:  {ADC(af.b.h);break;}

case 0x90:  {SUB(bc.b.h);break;}
case 0x91:  {SUB(bc.b.l);break;}
case 0x92:  {SUB(de.b.h);break;}
case 0x93:  {SUB(de.b.l);break;}
case 0x94:  {SUB(hl.b.h);break;}
case 0x95:  {SUB(hl.b.l);break;}
case 0x96:  {SUB(m.read_byte(hl.w));break;}
case 0x97:  {SUB(af.b.h);break;}

case 0x98:  {SBC(bc.b.h);break;}
case 0x99:  {SBC(bc.b.l);break;}
case 0x9a:  {SBC(de.b.h);break;}
case 0x9b:  {SBC(de.b.l);break;}
case 0x9c:  {SBC(hl.b.h);break;}
case 0x9d:  {SBC(hl.b.l);break;}
case 0x9e:  {SBC(m.read_byte(hl.w));break;}
case 0x9f:  {SBC(af.b.h);break;}

case 0xa0:  {AND(bc.b.h);break;}
case 0xa1:  {AND(bc.b.l);break;}
case 0xa2:  {AND(de.b.h);break;}
case 0xa3:  {AND(de.b.l);break;}
case 0xa4:  {AND(hl.b.h);break;}
case 0xa5:  {AND(hl.b.l);break;}
case 0xa6:  {AND(m.read_byte(hl.w));break;}
case 0xa7:  {AND(af.b.h);break;}

case 0xa8:  {EOR(bc.b.h);break;}
case 0xa9:  {EOR(bc.b.l);break;}
case 0xaa:  {EOR(de.b.h);break;}
case 0xab:  {EOR(de.b.l);break;}
case 0xac:  {EOR(hl.b.h);break;}
case 0xad:  {EOR(hl.b.l);break;}
case 0xae:  {EOR(m.read_byte(hl.w));break;}
case 0xaf:  {EOR(af.b.h);break;}

case 0xb0:  {OR(bc.b.h);break;}
case 0xb1:  {OR(bc.b.l);break;}
case 0xb2:  {OR(de.b.h);break;}
case 0xb3:  {OR(de.b.l);break;}
case 0xb4:  {OR(hl.b.h);break;}
case 0xb5:  {OR(hl.b.l);break;}
case 0xb6:  {OR(m.read_byte(hl.w));break;}
case 0xb7:  {OR(af.b.h);break;}

case 0xb8:  {CP(bc.b.h);break;}
case 0xb9:  {CP(bc.b.l);break;}
case 0xba:  {CP(de.b.h);break;}
case 0xbb:  {CP(de.b.l);break;}
case 0xbc:  {CP(hl.b.h);break;}
case 0xbd:  {CP(hl.b.l);break;}
case 0xbe:  {CP(m.read_byte(hl.w));break;}
case 0xbf:  {CP(af.b.h);break;}

case 0xc0:  {COND_RET(!(af.b.l & F_Z));break;}
case 0xc1:  {POP(bc.b.h,bc.b.l);break;}
case 0xc2:  {COND_JP(!(af.b.l & F_Z),m.read_word(pc.w-2));break;}
case 0xc3:  {JP(m.read_word(pc.w-2));break;}
case 0xc4:  {COND_CALL(!(af.b.l & F_Z),m.read_word(pc.w-2));break;}
case 0xc5:  {PUSH(bc.b.h,bc.b.l);break;}
case 0xc6:  {ADD(m.read_byte(pc.w-1));break;}
case 0xc7:  {RST(0x00);break;}

case 0xc8:  {COND_RET((af.b.l & F_Z));break;}
case 0xc9:  {RET;break;}
case 0xca:  {COND_JP((af.b.l & F_Z),m.read_word(pc.w-2));break;}
case 0xcb:  {break;}//cb opcodes
case 0xcc:  {COND_CALL((af.b.l & F_Z),m.read_word(pc.w-2));break;}
case 0xcd:  {CALL(m.read_word(pc.w-2));break;}
case 0xce:  {ADC(m.read_byte(pc.w-1));break;}
case 0xcf:  {RST(0x08);break;}

case 0xd0:  {COND_RET(!(af.b.l & F_C));break;}
case 0xd1:  {POP(de.b.h,de.b.l);break;}
case 0xd2:  {COND_JP(!(af.b.l & F_C),m.read_word(pc.w-2));break;}
case 0xd3:  {break;}//no opcode
case 0xd4:  {COND_CALL(!(af.b.l & F_C),m.read_word(pc.w-2));break;}
case 0xd5:  {PUSH(de.b.h,de.b.l);break;}
case 0xd6:  {SUB(m.read_byte(pc.w-1));break;}
case 0xd7:  {RST(0x10);break;}

case 0xd8:  {COND_RET((af.b.l & F_C));break;}
case 0xd9:  {RETI;break;}
case 0xda:  {COND_JP((af.b.l & F_C),m.read_word(pc.w-2));break;}
case 0xdb:  {break;}//no opcode
case 0xdc:  {COND_CALL((af.b.l & F_C),m.read_word(pc.w-2));break;}
case 0xdd:  {break;}//no opcode
case 0xde:  {SBC(m.read_byte(pc.w-1));break;}
case 0xdf:  {RST(0x18);break;}

case 0xe0:  {uint16 temp=m.read_byte(pc.w-1)+0xFF00;LD_RM(temp,af.b.h);break;}
case 0xe1:  {POP(hl.b.h,hl.b.l);break;}
case 0xe2:  {uint16 temp=bc.b.l+0xFF00;LD_RM(temp,af.b.h);break;}
case 0xe3:  {break;}//no opcode
case 0xe4:  {break;}//no opcode
case 0xe5:  {PUSH(hl.b.h,hl.b.l);break;}
case 0xe6:  {AND(m.read_byte(pc.w-1));break;}
case 0xe7:  {RST(0x20);break;}

case 0xe8:  {int mtemp = sp.w + ((signed char)m.read_byte(pc.w-1));af.b.l = (mtemp & 0x0100 ? F_C : 0)|((hl.w^(m.read_byte(pc.w-1))^(mtemp & 0xFFFF)) & 0x10 ? F_H : 0);sp.w = mtemp & 0xFFFF;break;}//add sp, r8
case 0xe9:  {JP(hl.w);break;}
case 0xea:  {LD_RM(m.read_word(pc.w-2),af.b.h);break;}
case 0xeb:  {break;}//no opcode
case 0xec:  {break;}//no opcode
case 0xed:  {break;}//no opcode
case 0xee:  {EOR(m.read_byte(pc.w-1));break;}
case 0xef:  {RST(0x28);break;}

case 0xf0:  {uint16 temp=m.read_byte(pc.w-1)+0xFF00;LD_MR(af.b.h,temp);break;}
case 0xf1:  {POP(af.b.h,af.b.l);break;}
case 0xf2:  {uint16 temp=bc.b.l+0xFF00;LD_MR(af.b.h,temp);break;}
case 0xf3:  {ime=0;ei_delay = 0;break;}
case 0xf4:  {break;}//no opcode
case 0xf5:  {PUSH(af.b.h,af.b.l);break;}
case 0xf6:  {OR(m.read_byte(pc.w-1));break;}
case 0xf7:  {RST(0x30);break;}

//case 0xf8:  {break;}//ld hl, sp+r8
case 0xf9:  {LD(sp.w,hl.w);break;}
case 0xfa:  {LD_MR(af.b.h,m.read_word(pc.w-2));break;}
case 0xfb:  {ei_delay = 1;break;}
case 0xfc:  {break;}//no opcode
case 0xfd:  {break;}//no opcode
case 0xfe:  {CP(m.read_byte(pc.w-1));break;}
case 0xff:  {RST(0x38);break;}

default:  {printf("opcode 0x%x not found\n",op);getchar();}
