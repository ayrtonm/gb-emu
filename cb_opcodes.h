case 0x0:  {RLC(_B);break;}
case 0x1:  {RLC(_C);break;}
case 0x2:  {RLC(_D);break;}
case 0x3:  {RLC(_E);break;}
case 0x4:  {RLC(_H);break;}
case 0x5:  {RLC(_L);break;}
case 0x6:  {uint8 temp = read_byte(_HL);RLC(temp);write_byte(_HL,temp);break;}
case 0x7:  {RLC(_A);break;}
case 0x8:  {RRC(_B);break;}
case 0x9:  {RRC(_C);break;}
case 0xa:  {RRC(_D);break;}
case 0xb:  {RRC(_E);break;}
case 0xc:  {RRC(_H);break;}
case 0xd:  {RRC(_L);break;}
case 0xe:  {uint8 temp = read_byte(_HL);RRC(temp);write_byte(_HL,temp);break;}
case 0xf:  {RRC(_A);break;}
case 0x10:  {RL(_B);break;}
case 0x11:  {RL(_C);break;}
case 0x12:  {RL(_D);break;}
case 0x13:  {RL(_E);break;}
case 0x14:  {RL(_H);break;}
case 0x15:  {RL(_L);break;}
case 0x16:  {uint8 temp = read_byte(_HL);RL(temp);write_byte(_HL,temp);break;}
case 0x17:  {RL(_A);break;}
case 0x18:  {RR(_B);break;}
case 0x19:  {RR(_C);break;}
case 0x1a:  {RR(_D);break;}
case 0x1b:  {RR(_E);break;}
case 0x1c:  {RR(_H);break;}
case 0x1d:  {RR(_L);break;}
case 0x1e:  {uint8 temp = read_byte(_HL);RR(temp);write_byte(_HL,temp);break;}
case 0x1f:  {RR(_A);break;}
case 0x20:  {SLA(_B);break;}
case 0x21:  {SLA(_C);break;}
case 0x22:  {SLA(_D);break;}
case 0x23:  {SLA(_E);break;}
case 0x24:  {SLA(_H);break;}
case 0x25:  {SLA(_L);break;}
case 0x26:  {uint8 temp = read_byte(_HL);SLA(temp);write_byte(_HL,temp);break;}
case 0x27:  {SLA(_A);break;}
case 0x28:  {SRA(_B);break;}
case 0x29:  {SRA(_C);break;}
case 0x2a:  {SRA(_D);break;}
case 0x2b:  {SRA(_E);break;}
case 0x2c:  {SRA(_H);break;}
case 0x2d:  {SRA(_L);break;}
case 0x2e:  {uint8 temp = read_byte(_HL);SRA(temp);write_byte(_HL,temp);break;}
case 0x2f:  {SRA(_A);break;}
case 0x30:  {CB_SWAP(_B);break;}
case 0x31:  {CB_SWAP(_C);break;}
case 0x32:  {CB_SWAP(_D);break;}
case 0x33:  {CB_SWAP(_E);break;}
case 0x34:  {CB_SWAP(_H);break;}
case 0x35:  {CB_SWAP(_L);break;}
case 0x36:  {uint8 temp = read_byte(_HL);CB_SWAP(temp);write_byte(_HL,temp);break;}
case 0x37:  {CB_SWAP(_A);break;}
case 0x38:  {SRL(_B);break;}
case 0x39:  {SRL(_C);break;}
case 0x3a:  {SRL(_D);break;}
case 0x3b:  {SRL(_E);break;}
case 0x3c:  {SRL(_H);break;}
case 0x3d:  {SRL(_L);break;}
case 0x3e:  {uint8 temp = read_byte(_HL);SRL(temp);write_byte(_HL,temp);break;}
case 0x3f:  {SRL(_A);break;}
case 0x40:  {(GET(0x1,_B) == 0 ? SET(Z_FLAG,_F) : CLEAR(Z_FLAG,_F));SET(H_FLAG,_F);CLEAR(N_FLAG,_F);break;}
case 0x41:  {(GET(0x1,_C) == 0 ? SET(Z_FLAG,_F) : CLEAR(Z_FLAG,_F));SET(H_FLAG,_F);CLEAR(N_FLAG,_F);break;}
case 0x42:  {(GET(0x1,_D) == 0 ? SET(Z_FLAG,_F) : CLEAR(Z_FLAG,_F));SET(H_FLAG,_F);CLEAR(N_FLAG,_F);break;}
case 0x43:  {(GET(0x1,_E) == 0 ? SET(Z_FLAG,_F) : CLEAR(Z_FLAG,_F));SET(H_FLAG,_F);CLEAR(N_FLAG,_F);break;}
case 0x44:  {(GET(0x1,_H) == 0 ? SET(Z_FLAG,_F) : CLEAR(Z_FLAG,_F));SET(H_FLAG,_F);CLEAR(N_FLAG,_F);break;}
case 0x45:  {(GET(0x1,_L) == 0 ? SET(Z_FLAG,_F) : CLEAR(Z_FLAG,_F));SET(H_FLAG,_F);CLEAR(N_FLAG,_F);break;}
case 0x46:  {(GET(0x1,read_byte(_HL)) == 0 ? SET(Z_FLAG,_F) : CLEAR(Z_FLAG,_F));SET(H_FLAG,_F);CLEAR(N_FLAG,_F);break;}
case 0x47:  {(GET(0x1,_A) == 0 ? SET(Z_FLAG,_F) : CLEAR(Z_FLAG,_F));SET(H_FLAG,_F);CLEAR(N_FLAG,_F);break;}
case 0x48:  {(GET(0x2,_B) == 0 ? SET(Z_FLAG,_F) : CLEAR(Z_FLAG,_F));SET(H_FLAG,_F);CLEAR(N_FLAG,_F);break;}
case 0x49:  {(GET(0x2,_C) == 0 ? SET(Z_FLAG,_F) : CLEAR(Z_FLAG,_F));SET(H_FLAG,_F);CLEAR(N_FLAG,_F);break;}
case 0x4a:  {(GET(0x2,_D) == 0 ? SET(Z_FLAG,_F) : CLEAR(Z_FLAG,_F));SET(H_FLAG,_F);CLEAR(N_FLAG,_F);break;}
case 0x4b:  {(GET(0x2,_E) == 0 ? SET(Z_FLAG,_F) : CLEAR(Z_FLAG,_F));SET(H_FLAG,_F);CLEAR(N_FLAG,_F);break;}
case 0x4c:  {(GET(0x2,_H) == 0 ? SET(Z_FLAG,_F) : CLEAR(Z_FLAG,_F));SET(H_FLAG,_F);CLEAR(N_FLAG,_F);break;}
case 0x4d:  {(GET(0x2,_L) == 0 ? SET(Z_FLAG,_F) : CLEAR(Z_FLAG,_F));SET(H_FLAG,_F);CLEAR(N_FLAG,_F);break;}
case 0x4e:  {(GET(0x2,read_byte(_HL)) == 0 ? SET(Z_FLAG,_F) : CLEAR(Z_FLAG,_F));SET(H_FLAG,_F);CLEAR(N_FLAG,_F);break;}
case 0x4f:  {(GET(0x2,_A) == 0 ? SET(Z_FLAG,_F) : CLEAR(Z_FLAG,_F));SET(H_FLAG,_F);CLEAR(N_FLAG,_F);break;}
case 0x50:  {(GET(0x4,_B) == 0 ? SET(Z_FLAG,_F) : CLEAR(Z_FLAG,_F));SET(H_FLAG,_F);CLEAR(N_FLAG,_F);break;}
case 0x51:  {(GET(0x4,_C) == 0 ? SET(Z_FLAG,_F) : CLEAR(Z_FLAG,_F));SET(H_FLAG,_F);CLEAR(N_FLAG,_F);break;}
case 0x52:  {(GET(0x4,_D) == 0 ? SET(Z_FLAG,_F) : CLEAR(Z_FLAG,_F));SET(H_FLAG,_F);CLEAR(N_FLAG,_F);break;}
case 0x53:  {(GET(0x4,_E) == 0 ? SET(Z_FLAG,_F) : CLEAR(Z_FLAG,_F));SET(H_FLAG,_F);CLEAR(N_FLAG,_F);break;}
case 0x54:  {(GET(0x4,_H) == 0 ? SET(Z_FLAG,_F) : CLEAR(Z_FLAG,_F));SET(H_FLAG,_F);CLEAR(N_FLAG,_F);break;}
case 0x55:  {(GET(0x4,_L) == 0 ? SET(Z_FLAG,_F) : CLEAR(Z_FLAG,_F));SET(H_FLAG,_F);CLEAR(N_FLAG,_F);break;}
case 0x56:  {(GET(0x4,read_byte(_HL)) == 0 ? SET(Z_FLAG,_F) : CLEAR(Z_FLAG,_F));SET(H_FLAG,_F);CLEAR(N_FLAG,_F);break;}
case 0x57:  {(GET(0x4,_A) == 0 ? SET(Z_FLAG,_F) : CLEAR(Z_FLAG,_F));SET(H_FLAG,_F);CLEAR(N_FLAG,_F);break;}
case 0x58:  {(GET(0x8,_B) == 0 ? SET(Z_FLAG,_F) : CLEAR(Z_FLAG,_F));SET(H_FLAG,_F);CLEAR(N_FLAG,_F);break;}
case 0x59:  {(GET(0x8,_C) == 0 ? SET(Z_FLAG,_F) : CLEAR(Z_FLAG,_F));SET(H_FLAG,_F);CLEAR(N_FLAG,_F);break;}
case 0x5a:  {(GET(0x8,_D) == 0 ? SET(Z_FLAG,_F) : CLEAR(Z_FLAG,_F));SET(H_FLAG,_F);CLEAR(N_FLAG,_F);break;}
case 0x5b:  {(GET(0x8,_E) == 0 ? SET(Z_FLAG,_F) : CLEAR(Z_FLAG,_F));SET(H_FLAG,_F);CLEAR(N_FLAG,_F);break;}
case 0x5c:  {(GET(0x8,_H) == 0 ? SET(Z_FLAG,_F) : CLEAR(Z_FLAG,_F));SET(H_FLAG,_F);CLEAR(N_FLAG,_F);break;}
case 0x5d:  {(GET(0x8,_L) == 0 ? SET(Z_FLAG,_F) : CLEAR(Z_FLAG,_F));SET(H_FLAG,_F);CLEAR(N_FLAG,_F);break;}
case 0x5e:  {(GET(0x8,read_byte(_HL)) == 0 ? SET(Z_FLAG,_F) : CLEAR(Z_FLAG,_F));SET(H_FLAG,_F);CLEAR(N_FLAG,_F);break;}
case 0x5f:  {(GET(0x8,_A) == 0 ? SET(Z_FLAG,_F) : CLEAR(Z_FLAG,_F));SET(H_FLAG,_F);CLEAR(N_FLAG,_F);break;}
case 0x60:  {(GET(0x10,_B) == 0 ? SET(Z_FLAG,_F) : CLEAR(Z_FLAG,_F));SET(H_FLAG,_F);CLEAR(N_FLAG,_F);break;}
case 0x61:  {(GET(0x10,_C) == 0 ? SET(Z_FLAG,_F) : CLEAR(Z_FLAG,_F));SET(H_FLAG,_F);CLEAR(N_FLAG,_F);break;}
case 0x62:  {(GET(0x10,_D) == 0 ? SET(Z_FLAG,_F) : CLEAR(Z_FLAG,_F));SET(H_FLAG,_F);CLEAR(N_FLAG,_F);break;}
case 0x63:  {(GET(0x10,_E) == 0 ? SET(Z_FLAG,_F) : CLEAR(Z_FLAG,_F));SET(H_FLAG,_F);CLEAR(N_FLAG,_F);break;}
case 0x64:  {(GET(0x10,_H) == 0 ? SET(Z_FLAG,_F) : CLEAR(Z_FLAG,_F));SET(H_FLAG,_F);CLEAR(N_FLAG,_F);break;}
case 0x65:  {(GET(0x10,_L) == 0 ? SET(Z_FLAG,_F) : CLEAR(Z_FLAG,_F));SET(H_FLAG,_F);CLEAR(N_FLAG,_F);break;}
case 0x66:  {(GET(0x10,read_byte(_HL)) == 0 ? SET(Z_FLAG,_F) : CLEAR(Z_FLAG,_F));SET(H_FLAG,_F);CLEAR(N_FLAG,_F);break;}
case 0x67:  {(GET(0x10,_A) == 0 ? SET(Z_FLAG,_F) : CLEAR(Z_FLAG,_F));SET(H_FLAG,_F);CLEAR(N_FLAG,_F);break;}
case 0x68:  {(GET(0x20,_B) == 0 ? SET(Z_FLAG,_F) : CLEAR(Z_FLAG,_F));SET(H_FLAG,_F);CLEAR(N_FLAG,_F);break;}
case 0x69:  {(GET(0x20,_C) == 0 ? SET(Z_FLAG,_F) : CLEAR(Z_FLAG,_F));SET(H_FLAG,_F);CLEAR(N_FLAG,_F);break;}
case 0x6a:  {(GET(0x20,_D) == 0 ? SET(Z_FLAG,_F) : CLEAR(Z_FLAG,_F));SET(H_FLAG,_F);CLEAR(N_FLAG,_F);break;}
case 0x6b:  {(GET(0x20,_E) == 0 ? SET(Z_FLAG,_F) : CLEAR(Z_FLAG,_F));SET(H_FLAG,_F);CLEAR(N_FLAG,_F);break;}
case 0x6c:  {(GET(0x20,_H) == 0 ? SET(Z_FLAG,_F) : CLEAR(Z_FLAG,_F));SET(H_FLAG,_F);CLEAR(N_FLAG,_F);break;}
case 0x6d:  {(GET(0x20,_L) == 0 ? SET(Z_FLAG,_F) : CLEAR(Z_FLAG,_F));SET(H_FLAG,_F);CLEAR(N_FLAG,_F);break;}
case 0x6e:  {(GET(0x20,read_byte(_HL)) == 0 ? SET(Z_FLAG,_F) : CLEAR(Z_FLAG,_F));SET(H_FLAG,_F);CLEAR(N_FLAG,_F);break;}
case 0x6f:  {(GET(0x20,_A) == 0 ? SET(Z_FLAG,_F) : CLEAR(Z_FLAG,_F));SET(H_FLAG,_F);CLEAR(N_FLAG,_F);break;}
case 0x70:  {(GET(0x40,_B) == 0 ? SET(Z_FLAG,_F) : CLEAR(Z_FLAG,_F));SET(H_FLAG,_F);CLEAR(N_FLAG,_F);break;}
case 0x71:  {(GET(0x40,_C) == 0 ? SET(Z_FLAG,_F) : CLEAR(Z_FLAG,_F));SET(H_FLAG,_F);CLEAR(N_FLAG,_F);break;}
case 0x72:  {(GET(0x40,_D) == 0 ? SET(Z_FLAG,_F) : CLEAR(Z_FLAG,_F));SET(H_FLAG,_F);CLEAR(N_FLAG,_F);break;}
case 0x73:  {(GET(0x40,_E) == 0 ? SET(Z_FLAG,_F) : CLEAR(Z_FLAG,_F));SET(H_FLAG,_F);CLEAR(N_FLAG,_F);break;}
case 0x74:  {(GET(0x40,_H) == 0 ? SET(Z_FLAG,_F) : CLEAR(Z_FLAG,_F));SET(H_FLAG,_F);CLEAR(N_FLAG,_F);break;}
case 0x75:  {(GET(0x40,_L) == 0 ? SET(Z_FLAG,_F) : CLEAR(Z_FLAG,_F));SET(H_FLAG,_F);CLEAR(N_FLAG,_F);break;}
case 0x76:  {(GET(0x40,read_byte(_HL)) == 0 ? SET(Z_FLAG,_F) : CLEAR(Z_FLAG,_F));SET(H_FLAG,_F);CLEAR(N_FLAG,_F);break;}
case 0x77:  {(GET(0x40,_A) == 0 ? SET(Z_FLAG,_F) : CLEAR(Z_FLAG,_F));SET(H_FLAG,_F);CLEAR(N_FLAG,_F);break;}
case 0x78:  {(GET(0x80,_B) == 0 ? SET(Z_FLAG,_F) : CLEAR(Z_FLAG,_F));SET(H_FLAG,_F);CLEAR(N_FLAG,_F);break;}
case 0x79:  {(GET(0x80,_C) == 0 ? SET(Z_FLAG,_F) : CLEAR(Z_FLAG,_F));SET(H_FLAG,_F);CLEAR(N_FLAG,_F);break;}
case 0x7a:  {(GET(0x80,_D) == 0 ? SET(Z_FLAG,_F) : CLEAR(Z_FLAG,_F));SET(H_FLAG,_F);CLEAR(N_FLAG,_F);break;}
case 0x7b:  {(GET(0x80,_E) == 0 ? SET(Z_FLAG,_F) : CLEAR(Z_FLAG,_F));SET(H_FLAG,_F);CLEAR(N_FLAG,_F);break;}
case 0x7c:  {(GET(0x80,_H) == 0 ? SET(Z_FLAG,_F) : CLEAR(Z_FLAG,_F));SET(H_FLAG,_F);CLEAR(N_FLAG,_F);break;}
case 0x7d:  {(GET(0x80,_L) == 0 ? SET(Z_FLAG,_F) : CLEAR(Z_FLAG,_F));SET(H_FLAG,_F);CLEAR(N_FLAG,_F);break;}
case 0x7e:  {(GET(0x80,read_byte(_HL)) == 0 ? SET(Z_FLAG,_F) : CLEAR(Z_FLAG,_F));SET(H_FLAG,_F);CLEAR(N_FLAG,_F);break;}
case 0x7f:  {(GET(0x80,_A) == 0 ? SET(Z_FLAG,_F) : CLEAR(Z_FLAG,_F));SET(H_FLAG,_F);CLEAR(N_FLAG,_F);break;}
case 0x80:  {CLEAR(0x1,_B);break;}
case 0x81:  {CLEAR(0x1,_C);break;}
case 0x82:  {CLEAR(0x1,_D);break;}
case 0x83:  {CLEAR(0x1,_E);break;}
case 0x84:  {CLEAR(0x1,_H);break;}
case 0x85:  {CLEAR(0x1,_L);break;}
case 0x86:  {uint8 temp = read_byte(_HL);CLEAR(0x1,temp);write_byte(_HL,temp);break;}
case 0x87:  {CLEAR(0x1,_A);break;}
case 0x88:  {CLEAR(0x2,_B);break;}
case 0x89:  {CLEAR(0x2,_C);break;}
case 0x8a:  {CLEAR(0x2,_D);break;}
case 0x8b:  {CLEAR(0x2,_E);break;}
case 0x8c:  {CLEAR(0x2,_H);break;}
case 0x8d:  {CLEAR(0x2,_L);break;}
case 0x8e:  {uint8 temp = read_byte(_HL);CLEAR(0x2,temp);write_byte(_HL,temp);break;}
case 0x8f:  {CLEAR(0x2,_A);break;}
case 0x90:  {CLEAR(0x4,_B);break;}
case 0x91:  {CLEAR(0x4,_C);break;}
case 0x92:  {CLEAR(0x4,_D);break;}
case 0x93:  {CLEAR(0x4,_E);break;}
case 0x94:  {CLEAR(0x4,_H);break;}
case 0x95:  {CLEAR(0x4,_L);break;}
case 0x96:  {uint8 temp = read_byte(_HL);CLEAR(0x4,temp);write_byte(_HL,temp);break;}
case 0x97:  {CLEAR(0x4,_A);break;}
case 0x98:  {CLEAR(0x8,_B);break;}
case 0x99:  {CLEAR(0x8,_C);break;}
case 0x9a:  {CLEAR(0x8,_D);break;}
case 0x9b:  {CLEAR(0x8,_E);break;}
case 0x9c:  {CLEAR(0x8,_H);break;}
case 0x9d:  {CLEAR(0x8,_L);break;}
case 0x9e:  {uint8 temp = read_byte(_HL);CLEAR(0x8,temp);write_byte(_HL,temp);break;}
case 0x9f:  {CLEAR(0x8,_A);break;}
case 0xa0:  {CLEAR(0x10,_B);break;}
case 0xa1:  {CLEAR(0x10,_C);break;}
case 0xa2:  {CLEAR(0x10,_D);break;}
case 0xa3:  {CLEAR(0x10,_E);break;}
case 0xa4:  {CLEAR(0x10,_H);break;}
case 0xa5:  {CLEAR(0x10,_L);break;}
case 0xa6:  {uint8 temp = read_byte(_HL);CLEAR(0x10,temp);write_byte(_HL,temp);break;}
case 0xa7:  {CLEAR(0x10,_A);break;}
case 0xa8:  {CLEAR(0x20,_B);break;}
case 0xa9:  {CLEAR(0x20,_C);break;}
case 0xaa:  {CLEAR(0x20,_D);break;}
case 0xab:  {CLEAR(0x20,_E);break;}
case 0xac:  {CLEAR(0x20,_H);break;}
case 0xad:  {CLEAR(0x20,_L);break;}
case 0xae:  {uint8 temp = read_byte(_HL);CLEAR(0x20,temp);write_byte(_HL,temp);break;}
case 0xaf:  {CLEAR(0x20,_A);break;}
case 0xb0:  {CLEAR(0x40,_B);break;}
case 0xb1:  {CLEAR(0x40,_C);break;}
case 0xb2:  {CLEAR(0x40,_D);break;}
case 0xb3:  {CLEAR(0x40,_E);break;}
case 0xb4:  {CLEAR(0x40,_H);break;}
case 0xb5:  {CLEAR(0x40,_L);break;}
case 0xb6:  {uint8 temp = read_byte(_HL);CLEAR(0x40,temp);write_byte(_HL,temp);break;}
case 0xb7:  {CLEAR(0x40,_A);break;}
case 0xb8:  {CLEAR(0x80,_B);break;}
case 0xb9:  {CLEAR(0x80,_C);break;}
case 0xba:  {CLEAR(0x80,_D);break;}
case 0xbb:  {CLEAR(0x80,_E);break;}
case 0xbc:  {CLEAR(0x80,_H);break;}
case 0xbd:  {CLEAR(0x80,_L);break;}
case 0xbe:  {uint8 temp = read_byte(_HL);CLEAR(0x80,temp);write_byte(_HL,temp);break;}
case 0xbf:  {CLEAR(0x80,_A);break;}
case 0xc0:  {SET(0x1,_B);break;}
case 0xc1:  {SET(0x1,_C);break;}
case 0xc2:  {SET(0x1,_D);break;}
case 0xc3:  {SET(0x1,_E);break;}
case 0xc4:  {SET(0x1,_H);break;}
case 0xc5:  {SET(0x1,_L);break;}
case 0xc6:  {uint8 temp = read_byte(_HL);SET(0x1,temp);write_byte(_HL,temp);break;}
case 0xc7:  {SET(0x1,_A);break;}
case 0xc8:  {SET(0x2,_B);break;}
case 0xc9:  {SET(0x2,_C);break;}
case 0xca:  {SET(0x2,_D);break;}
case 0xcb:  {SET(0x2,_E);break;}
case 0xcc:  {SET(0x2,_H);break;}
case 0xcd:  {SET(0x2,_L);break;}
case 0xce:  {uint8 temp = read_byte(_HL);SET(0x2,temp);write_byte(_HL,temp);break;}
case 0xcf:  {SET(0x2,_A);break;}
case 0xd0:  {SET(0x4,_B);break;}
case 0xd1:  {SET(0x4,_C);break;}
case 0xd2:  {SET(0x4,_D);break;}
case 0xd3:  {SET(0x4,_E);break;}
case 0xd4:  {SET(0x4,_H);break;}
case 0xd5:  {SET(0x4,_L);break;}
case 0xd6:  {uint8 temp = read_byte(_HL);SET(0x4,temp);write_byte(_HL,temp);break;}
case 0xd7:  {SET(0x4,_A);break;}
case 0xd8:  {SET(0x8,_B);break;}
case 0xd9:  {SET(0x8,_C);break;}
case 0xda:  {SET(0x8,_D);break;}
case 0xdb:  {SET(0x8,_E);break;}
case 0xdc:  {SET(0x8,_H);break;}
case 0xdd:  {SET(0x8,_L);break;}
case 0xde:  {uint8 temp = read_byte(_HL);SET(0x8,temp);write_byte(_HL,temp);break;}
case 0xdf:  {SET(0x8,_A);break;}
case 0xe0:  {SET(0x10,_B);break;}
case 0xe1:  {SET(0x10,_C);break;}
case 0xe2:  {SET(0x10,_D);break;}
case 0xe3:  {SET(0x10,_E);break;}
case 0xe4:  {SET(0x10,_H);break;}
case 0xe5:  {SET(0x10,_L);break;}
case 0xe6:  {uint8 temp = read_byte(_HL);SET(0x10,temp);write_byte(_HL,temp);break;}
case 0xe7:  {SET(0x10,_A);break;}
case 0xe8:  {SET(0x20,_B);break;}
case 0xe9:  {SET(0x20,_C);break;}
case 0xea:  {SET(0x20,_D);break;}
case 0xeb:  {SET(0x20,_E);break;}
case 0xec:  {SET(0x20,_H);break;}
case 0xed:  {SET(0x20,_L);break;}
case 0xee:  {uint8 temp = read_byte(_HL);SET(0x20,temp);write_byte(_HL,temp);break;}
case 0xef:  {SET(0x20,_A);break;}
case 0xf0:  {SET(0x40,_B);break;}
case 0xf1:  {SET(0x40,_C);break;}
case 0xf2:  {SET(0x40,_D);break;}
case 0xf3:  {SET(0x40,_E);break;}
case 0xf4:  {SET(0x40,_H);break;}
case 0xf5:  {SET(0x40,_L);break;}
case 0xf6:  {uint8 temp = read_byte(_HL);SET(0x40,temp);write_byte(_HL,temp);break;}
case 0xf7:  {SET(0x40,_A);break;}
case 0xf8:  {SET(0x80,_B);break;}
case 0xf9:  {SET(0x80,_C);break;}
case 0xfa:  {SET(0x80,_D);break;}
case 0xfb:  {SET(0x80,_E);break;}
case 0xfc:  {SET(0x80,_H);break;}
case 0xfd:  {SET(0x80,_L);break;}
case 0xfe:  {uint8 temp = read_byte(_HL);SET(0x80,temp);write_byte(_HL,temp);break;}
case 0xff:  {SET(0x80,_A);break;}
default:  {char string[22];snprintf(string,22,"cb opcode 0x%x not found\n",opcode);write_log(string);getchar();}
