case 0x0:  {RLC(_B);break;}
case 0x1:  {RLC(_C);break;}
case 0x2:  {RLC(_D);break;}
case 0x3:  {RLC(_E);break;}
case 0x4:  {RLC(_H);break;}
case 0x5:  {RLC(_L);break;}
case 0x6:  {uint8 temp = READ_BYTE(_HL);RLC(temp);write_byte(_HL,temp);break;}
case 0x7:  {RLC(_A);break;}
case 0x8:  {RRC(_B);break;}
case 0x9:  {RRC(_C);break;}
case 0xa:  {RRC(_D);break;}
case 0xb:  {RRC(_E);break;}
case 0xc:  {RRC(_H);break;}
case 0xd:  {RRC(_L);break;}
case 0xe:  {uint8 temp = READ_BYTE(_HL);RRC(temp);write_byte(_HL,temp);break;}
case 0xf:  {RRC(_A);break;}
case 0x10:  {RL(_B);break;}
case 0x11:  {RL(_C);break;}
case 0x12:  {RL(_D);break;}
case 0x13:  {RL(_E);break;}
case 0x14:  {RL(_H);break;}
case 0x15:  {RL(_L);break;}
case 0x16:  {uint8 temp = READ_BYTE(_HL);RL(temp);write_byte(_HL,temp);break;}
case 0x17:  {RL(_A);break;}
case 0x18:  {RR(_B);break;}
case 0x19:  {RR(_C);break;}
case 0x1a:  {RR(_D);break;}
case 0x1b:  {RR(_E);break;}
case 0x1c:  {RR(_H);break;}
case 0x1d:  {RR(_L);break;}
case 0x1e:  {uint8 temp = READ_BYTE(_HL);RR(temp);write_byte(_HL,temp);break;}
case 0x1f:  {RR(_A);break;}
case 0x20:  {SLA(_B);break;}
case 0x21:  {SLA(_C);break;}
case 0x22:  {SLA(_D);break;}
case 0x23:  {SLA(_E);break;}
case 0x24:  {SLA(_H);break;}
case 0x25:  {SLA(_L);break;}
case 0x26:  {uint8 temp = READ_BYTE(_HL);SLA(temp);write_byte(_HL,temp);break;}
case 0x27:  {SLA(_A);break;}
case 0x28:  {SRA(_B);break;}
case 0x29:  {SRA(_C);break;}
case 0x2a:  {SRA(_D);break;}
case 0x2b:  {SRA(_E);break;}
case 0x2c:  {SRA(_H);break;}
case 0x2d:  {SRA(_L);break;}
case 0x2e:  {uint8 temp = READ_BYTE(_HL);SRA(temp);write_byte(_HL,temp);break;}
case 0x2f:  {SRA(_A);break;}
case 0x30:  {CB_SWAP(_B);break;}
case 0x31:  {CB_SWAP(_C);break;}
case 0x32:  {CB_SWAP(_D);break;}
case 0x33:  {CB_SWAP(_E);break;}
case 0x34:  {CB_SWAP(_H);break;}
case 0x35:  {CB_SWAP(_L);break;}
case 0x36:  {uint8 temp = READ_BYTE(_HL);CB_SWAP(temp);write_byte(_HL,temp);break;}
case 0x37:  {CB_SWAP(_A);break;}
case 0x38:  {SRL(_B);break;}
case 0x39:  {SRL(_C);break;}
case 0x3a:  {SRL(_D);break;}
case 0x3b:  {SRL(_E);break;}
case 0x3c:  {SRL(_H);break;}
case 0x3d:  {SRL(_L);break;}
case 0x3e:  {uint8 temp = READ_BYTE(_HL);SRL(temp);write_byte(_HL,temp);break;}
case 0x3f:  {SRL(_A);break;}
case 0x40:  {_F = ((_B & 0x01) ? Z_FLAG : 0)|H_FLAG|(C_FLAG & _F);break;}
case 0x41:  {_F = ((_C & 0x01) ? Z_FLAG : 0)|H_FLAG|(C_FLAG & _F);break;}
case 0x42:  {_F = ((_D & 0x01) ? Z_FLAG : 0)|H_FLAG|(C_FLAG & _F);break;}
case 0x43:  {_F = ((_E & 0x01) ? Z_FLAG : 0)|H_FLAG|(C_FLAG & _F);break;}
case 0x44:  {_F = ((_H & 0x01) ? Z_FLAG : 0)|H_FLAG|(C_FLAG & _F);break;}
case 0x45:  {_F = ((_L & 0x01) ? Z_FLAG : 0)|H_FLAG|(C_FLAG & _F);break;}
case 0x46:  {_F = ((READ_BYTE(_HL) & 0x01) ? Z_FLAG : 0)|H_FLAG|(C_FLAG & _F);break;}
case 0x47:  {_F = ((_A & 0x01) ? Z_FLAG : 0)|H_FLAG|(C_FLAG & _F);break;}
case 0x48:  {_F = ((_B & 0x02) ? Z_FLAG : 0)|H_FLAG|(C_FLAG & _F);break;}
case 0x49:  {_F = ((_C & 0x02) ? Z_FLAG : 0)|H_FLAG|(C_FLAG & _F);break;}
case 0x4A:  {_F = ((_D & 0x02) ? Z_FLAG : 0)|H_FLAG|(C_FLAG & _F);break;}
case 0x4B:  {_F = ((_E & 0x02) ? Z_FLAG : 0)|H_FLAG|(C_FLAG & _F);break;}
case 0x4C:  {_F = ((_H & 0x02) ? Z_FLAG : 0)|H_FLAG|(C_FLAG & _F);break;}
case 0x4D:  {_F = ((_L & 0x02) ? Z_FLAG : 0)|H_FLAG|(C_FLAG & _F);break;}
case 0x4E:  {_F = ((READ_BYTE(_HL) & 0x02) ? Z_FLAG : 0)|H_FLAG|(C_FLAG & _F);break;}
case 0x4F:  {_F = ((_A & 0x02) ? Z_FLAG : 0)|H_FLAG|(C_FLAG & _F);break;}
case 0x50:  {_F = ((_B & 0x04) ? Z_FLAG : 0)|H_FLAG|(C_FLAG & _F);break;}
case 0x51:  {_F = ((_C & 0x04) ? Z_FLAG : 0)|H_FLAG|(C_FLAG & _F);break;}
case 0x52:  {_F = ((_D & 0x04) ? Z_FLAG : 0)|H_FLAG|(C_FLAG & _F);break;}
case 0x53:  {_F = ((_E & 0x04) ? Z_FLAG : 0)|H_FLAG|(C_FLAG & _F);break;}
case 0x54:  {_F = ((_H & 0x04) ? Z_FLAG : 0)|H_FLAG|(C_FLAG & _F);break;}
case 0x55:  {_F = ((_L & 0x04) ? Z_FLAG : 0)|H_FLAG|(C_FLAG & _F);break;}
case 0x56:  {_F = ((READ_BYTE(_HL) & 0x04) ? Z_FLAG : 0)|H_FLAG|(C_FLAG & _F);break;}
case 0x57:  {_F = ((_A & 0x04) ? Z_FLAG : 0)|H_FLAG|(C_FLAG & _F);break;}
case 0x58:  {_F = ((_B & 0x08) ? Z_FLAG : 0)|H_FLAG|(C_FLAG & _F);break;}
case 0x59:  {_F = ((_C & 0x08) ? Z_FLAG : 0)|H_FLAG|(C_FLAG & _F);break;}
case 0x5A:  {_F = ((_D & 0x08) ? Z_FLAG : 0)|H_FLAG|(C_FLAG & _F);break;}
case 0x5B:  {_F = ((_E & 0x08) ? Z_FLAG : 0)|H_FLAG|(C_FLAG & _F);break;}
case 0x5C:  {_F = ((_H & 0x08) ? Z_FLAG : 0)|H_FLAG|(C_FLAG & _F);break;}
case 0x5D:  {_F = ((_L & 0x08) ? Z_FLAG : 0)|H_FLAG|(C_FLAG & _F);break;}
case 0x5E:  {_F = ((READ_BYTE(_HL) & 0x08) ? Z_FLAG : 0)|H_FLAG|(C_FLAG & _F);break;}
case 0x5F:  {_F = ((_A & 0x08) ? Z_FLAG : 0)|H_FLAG|(C_FLAG & _F);break;}
case 0x60:  {_F = ((_B & 0x10) ? Z_FLAG : 0)|H_FLAG|(C_FLAG & _F);break;}
case 0x61:  {_F = ((_C & 0x10) ? Z_FLAG : 0)|H_FLAG|(C_FLAG & _F);break;}
case 0x62:  {_F = ((_D & 0x10) ? Z_FLAG : 0)|H_FLAG|(C_FLAG & _F);break;}
case 0x63:  {_F = ((_E & 0x10) ? Z_FLAG : 0)|H_FLAG|(C_FLAG & _F);break;}
case 0x64:  {_F = ((_H & 0x10) ? Z_FLAG : 0)|H_FLAG|(C_FLAG & _F);break;}
case 0x65:  {_F = ((_L & 0x10) ? Z_FLAG : 0)|H_FLAG|(C_FLAG & _F);break;}
case 0x66:  {_F = ((READ_BYTE(_HL) & 0x10) ? Z_FLAG : 0)|H_FLAG|(C_FLAG & _F);break;}
case 0x67:  {_F = ((_A & 0x10) ? Z_FLAG : 0)|H_FLAG|(C_FLAG & _F);break;}
case 0x68:  {_F = ((_B & 0x20) ? Z_FLAG : 0)|H_FLAG|(C_FLAG & _F);break;}
case 0x69:  {_F = ((_C & 0x20) ? Z_FLAG : 0)|H_FLAG|(C_FLAG & _F);break;}
case 0x6A:  {_F = ((_D & 0x20) ? Z_FLAG : 0)|H_FLAG|(C_FLAG & _F);break;}
case 0x6B:  {_F = ((_E & 0x20) ? Z_FLAG : 0)|H_FLAG|(C_FLAG & _F);break;}
case 0x6C:  {_F = ((_H & 0x20) ? Z_FLAG : 0)|H_FLAG|(C_FLAG & _F);break;}
case 0x6D:  {_F = ((_L & 0x20) ? Z_FLAG : 0)|H_FLAG|(C_FLAG & _F);break;}
case 0x6E:  {_F = ((READ_BYTE(_HL) & 0x20) ? Z_FLAG : 0)|H_FLAG|(C_FLAG & _F);break;}
case 0x6F:  {_F = ((_A & 0x20) ? Z_FLAG : 0)|H_FLAG|(C_FLAG & _F);break;}
case 0x70:  {_F = ((_B & 0x40) ? Z_FLAG : 0)|H_FLAG|(C_FLAG & _F);break;}
case 0x71:  {_F = ((_C & 0x40) ? Z_FLAG : 0)|H_FLAG|(C_FLAG & _F);break;}
case 0x72:  {_F = ((_D & 0x40) ? Z_FLAG : 0)|H_FLAG|(C_FLAG & _F);break;}
case 0x73:  {_F = ((_E & 0x40) ? Z_FLAG : 0)|H_FLAG|(C_FLAG & _F);break;}
case 0x74:  {_F = ((_H & 0x40) ? Z_FLAG : 0)|H_FLAG|(C_FLAG & _F);break;}
case 0x75:  {_F = ((_L & 0x40) ? Z_FLAG : 0)|H_FLAG|(C_FLAG & _F);break;}
case 0x76:  {_F = ((READ_BYTE(_HL) & 0x40) ? Z_FLAG : 0)|H_FLAG|(C_FLAG & _F);break;}
case 0x77:  {_F = ((_A & 0x40) ? Z_FLAG : 0)|H_FLAG|(C_FLAG & _F);break;}
case 0x78:  {_F = ((_B & 0x80) ? Z_FLAG : 0)|H_FLAG|(C_FLAG & _F);break;}
case 0x79:  {_F = ((_C & 0x80) ? Z_FLAG : 0)|H_FLAG|(C_FLAG & _F);break;}
case 0x7A:  {_F = ((_D & 0x80) ? Z_FLAG : 0)|H_FLAG|(C_FLAG & _F);break;}
case 0x7B:  {_F = ((_E & 0x80) ? Z_FLAG : 0)|H_FLAG|(C_FLAG & _F);break;}
case 0x7C:  {_F = ((_H & 0x80) ? Z_FLAG : 0)|H_FLAG|(C_FLAG & _F);break;}
case 0x7D:  {_F = ((_L & 0x80) ? Z_FLAG : 0)|H_FLAG|(C_FLAG & _F);break;}
case 0x7E:  {_F = ((READ_BYTE(_HL) & 0x80) ? Z_FLAG : 0)|H_FLAG|(C_FLAG & _F);break;}
case 0x7F:  {_F = ((_A & 0x80) ? Z_FLAG : 0)|H_FLAG|(C_FLAG & _F);break;}
case 0x80:  {_B = CLEAR(0x1,_B);break;}
case 0x81:  {_C = CLEAR(0x1,_C);break;}
case 0x82:  {_D = CLEAR(0x1,_D);break;}
case 0x83:  {_E = CLEAR(0x1,_E);break;}
case 0x84:  {_H = CLEAR(0x1,_H);break;}
case 0x85:  {_L = CLEAR(0x1,_L);break;}
case 0x86:  {uint8 temp = READ_BYTE(_HL);temp = CLEAR(0x1,temp);write_byte(_HL,temp);break;}
case 0x87:  {_A = CLEAR(0x1,_A);break;}
case 0x88:  {_B = CLEAR(0x2,_B);break;}
case 0x89:  {_C = CLEAR(0x2,_C);break;}
case 0x8A:  {_D = CLEAR(0x2,_D);break;}
case 0x8B:  {_E = CLEAR(0x2,_E);break;}
case 0x8C:  {_H = CLEAR(0x2,_H);break;}
case 0x8D:  {_L = CLEAR(0x2,_L);break;}
case 0x8E:  {uint8 temp = READ_BYTE(_HL);temp = CLEAR(0x2,temp);write_byte(_HL,temp);break;}
case 0x8F:  {_A = CLEAR(0x2,_A);break;}
case 0x90:  {_B = CLEAR(0x4,_B);break;}
case 0x91:  {_C = CLEAR(0x4,_C);break;}
case 0x92:  {_D = CLEAR(0x4,_D);break;}
case 0x93:  {_E = CLEAR(0x4,_E);break;}
case 0x94:  {_H = CLEAR(0x4,_H);break;}
case 0x95:  {_L = CLEAR(0x4,_L);break;}
case 0x96:  {uint8 temp = READ_BYTE(_HL);temp = CLEAR(0x4,temp);write_byte(_HL,temp);break;}
case 0x97:  {_A = CLEAR(0x4,_A);break;}
case 0x98:  {_B = CLEAR(0x8,_B);break;}
case 0x99:  {_C = CLEAR(0x8,_C);break;}
case 0x9A:  {_D = CLEAR(0x8,_D);break;}
case 0x9B:  {_E = CLEAR(0x8,_E);break;}
case 0x9C:  {_H = CLEAR(0x8,_H);break;}
case 0x9D:  {_L = CLEAR(0x8,_L);break;}
case 0x9E:  {uint8 temp = READ_BYTE(_HL);temp = CLEAR(0x8,temp);write_byte(_HL,temp);break;}
case 0x9F:  {_A = CLEAR(0x8,_A);break;}
case 0xa0:  {_B = CLEAR(0x10,_B);break;}
case 0xa1:  {_C = CLEAR(0x10,_C);break;}
case 0xa2:  {_D = CLEAR(0x10,_D);break;}
case 0xa3:  {_E = CLEAR(0x10,_E);break;}
case 0xa4:  {_H = CLEAR(0x10,_H);break;}
case 0xa5:  {_L = CLEAR(0x10,_L);break;}
case 0xa6:  {uint8 temp = READ_BYTE(_HL);temp = CLEAR(0x10,temp);write_byte(_HL,temp);break;}
case 0xa7:  {_A = CLEAR(0x10,_A);break;}
case 0xa8:  {_B = CLEAR(0x20,_B);break;}
case 0xa9:  {_C = CLEAR(0x20,_C);break;}
case 0xaA:  {_D = CLEAR(0x20,_D);break;}
case 0xaB:  {_E = CLEAR(0x20,_E);break;}
case 0xaC:  {_H = CLEAR(0x20,_H);break;}
case 0xaD:  {_L = CLEAR(0x20,_L);break;}
case 0xaE:  {uint8 temp = READ_BYTE(_HL);temp = CLEAR(0x20,temp);write_byte(_HL,temp);break;}
case 0xaF:  {_A = CLEAR(0x20,_A);break;}
case 0xb0:  {_B = CLEAR(0x40,_B);break;}
case 0xb1:  {_C = CLEAR(0x40,_C);break;}
case 0xb2:  {_D = CLEAR(0x40,_D);break;}
case 0xb3:  {_E = CLEAR(0x40,_E);break;}
case 0xb4:  {_H = CLEAR(0x40,_H);break;}
case 0xb5:  {_L = CLEAR(0x40,_L);break;}
case 0xb6:  {uint8 temp = READ_BYTE(_HL);temp = CLEAR(0x40,temp);write_byte(_HL,temp);break;}
case 0xb7:  {_A = CLEAR(0x40,_A);break;}
case 0xb8:  {_B = CLEAR(0x80,_B);break;}
case 0xb9:  {_C = CLEAR(0x80,_C);break;}
case 0xbA:  {_D = CLEAR(0x80,_D);break;}
case 0xbB:  {_E = CLEAR(0x80,_E);break;}
case 0xbC:  {_H = CLEAR(0x80,_H);break;}
case 0xbD:  {_L = CLEAR(0x80,_L);break;}
case 0xbE:  {uint8 temp = READ_BYTE(_HL);temp = CLEAR(0x80,temp);write_byte(_HL,temp);break;}
case 0xbF:  {_A = CLEAR(0x80,_A);break;}
case 0xc0:  {_B = SET(0x1,_B);break;}
case 0xc1:  {_C = SET(0x1,_C);break;}
case 0xc2:  {_D = SET(0x1,_D);break;}
case 0xc3:  {_E = SET(0x1,_E);break;}
case 0xc4:  {_H = SET(0x1,_H);break;}
case 0xc5:  {_L = SET(0x1,_L);break;}
case 0xc6:  {uint8 temp = READ_BYTE(_HL);temp = SET(0x1,temp);write_byte(_HL,temp);break;}
case 0xc7:  {_A = SET(0x1,_A);break;}
case 0xc8:  {_B = SET(0x2,_B);break;}
case 0xc9:  {_C = SET(0x2,_C);break;}
case 0xcA:  {_D = SET(0x2,_D);break;}
case 0xcB:  {_E = SET(0x2,_E);break;}
case 0xcC:  {_H = SET(0x2,_H);break;}
case 0xcD:  {_L = SET(0x2,_L);break;}
case 0xcE:  {uint8 temp = READ_BYTE(_HL);temp = SET(0x2,temp);write_byte(_HL,temp);break;}
case 0xcF:  {_A = SET(0x2,_A);break;}
case 0xd0:  {_B = SET(0x4,_B);break;}
case 0xd1:  {_C = SET(0x4,_C);break;}
case 0xd2:  {_D = SET(0x4,_D);break;}
case 0xd3:  {_E = SET(0x4,_E);break;}
case 0xd4:  {_H = SET(0x4,_H);break;}
case 0xd5:  {_L = SET(0x4,_L);break;}
case 0xd6:  {uint8 temp = READ_BYTE(_HL);temp = SET(0x4,temp);write_byte(_HL,temp);break;}
case 0xd7:  {_A = SET(0x4,_A);break;}
case 0xd8:  {_B = SET(0x8,_B);break;}
case 0xd9:  {_C = SET(0x8,_C);break;}
case 0xdA:  {_D = SET(0x8,_D);break;}
case 0xdB:  {_E = SET(0x8,_E);break;}
case 0xdC:  {_H = SET(0x8,_H);break;}
case 0xdD:  {_L = SET(0x8,_L);break;}
case 0xdE:  {uint8 temp = READ_BYTE(_HL);temp = SET(0x8,temp);write_byte(_HL,temp);break;}
case 0xdF:  {_A = SET(0x8,_A);break;}
case 0xe0:  {_B = SET(0x10,_B);break;}
case 0xe1:  {_C = SET(0x10,_C);break;}
case 0xe2:  {_D = SET(0x10,_D);break;}
case 0xe3:  {_E = SET(0x10,_E);break;}
case 0xe4:  {_H = SET(0x10,_H);break;}
case 0xe5:  {_L = SET(0x10,_L);break;}
case 0xe6:  {uint8 temp = READ_BYTE(_HL);temp = SET(0x10,temp);write_byte(_HL,temp);break;}
case 0xe7:  {_A = SET(0x10,_A);break;}
case 0xe8:  {_B = SET(0x20,_B);break;}
case 0xe9:  {_C = SET(0x20,_C);break;}
case 0xeA:  {_D = SET(0x20,_D);break;}
case 0xeB:  {_E = SET(0x20,_E);break;}
case 0xeC:  {_H = SET(0x20,_H);break;}
case 0xeD:  {_L = SET(0x20,_L);break;}
case 0xeE:  {uint8 temp = READ_BYTE(_HL);temp = SET(0x20,temp);write_byte(_HL,temp);break;}
case 0xeF:  {_A = SET(0x20,_A);break;}
case 0xf0:  {_B = SET(0x40,_B);break;}
case 0xf1:  {_C = SET(0x40,_C);break;}
case 0xf2:  {_D = SET(0x40,_D);break;}
case 0xf3:  {_E = SET(0x40,_E);break;}
case 0xf4:  {_H = SET(0x40,_H);break;}
case 0xf5:  {_L = SET(0x40,_L);break;}
case 0xf6:  {uint8 temp = READ_BYTE(_HL);temp = SET(0x40,temp);write_byte(_HL,temp);break;}
case 0xf7:  {_A = SET(0x40,_A);break;}
case 0xf8:  {_B = SET(0x80,_B);break;}
case 0xf9:  {_C = SET(0x80,_C);break;}
case 0xfA:  {_D = SET(0x80,_D);break;}
case 0xfB:  {_E = SET(0x80,_E);break;}
case 0xfC:  {_H = SET(0x80,_H);break;}
case 0xfD:  {_L = SET(0x80,_L);break;}
case 0xfE:  {uint8 temp = READ_BYTE(_HL);temp = SET(0x80,temp);write_byte(_HL,temp);break;}
case 0xfF:  {_A = SET(0x80,_A);break;}
default:  {printf("cb opcode 0x%x not found\n",op);getchar();}
