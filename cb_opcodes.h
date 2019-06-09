case 0x0:  {RLC(bc.b.h);break;}
case 0x1:  {RLC(bc.b.l);break;}
case 0x2:  {RLC(de.b.h);break;}
case 0x3:  {RLC(de.b.l);break;}
case 0x4:  {RLC(hl.b.h);break;}
case 0x5:  {RLC(hl.b.l);break;}
case 0x6:  {uint8 temp = m.read_byte(hl.w);RLC(temp);m.write_byte(hl.w,temp);break;}
case 0x7:  {RLC(af.b.h);break;}
case 0x8:  {RRC(bc.b.h);break;}
case 0x9:  {RRC(bc.b.l);break;}
case 0xa:  {RRC(de.b.h);break;}
case 0xb:  {RRC(de.b.l);break;}
case 0xc:  {RRC(hl.b.h);break;}
case 0xd:  {RRC(hl.b.l);break;}
case 0xe:  {uint8 temp = m.read_byte(hl.w);RRC(temp);m.write_byte(hl.w,temp);break;}
case 0xf:  {RRC(af.b.h);break;}
case 0x10:  {RL(bc.b.h);break;}
case 0x11:  {RL(bc.b.l);break;}
case 0x12:  {RL(de.b.h);break;}
case 0x13:  {RL(de.b.l);break;}
case 0x14:  {RL(hl.b.h);break;}
case 0x15:  {RL(hl.b.l);break;}
case 0x16:  {uint8 temp = m.read_byte(hl.w);RL(temp);m.write_byte(hl.w,temp);break;}
case 0x17:  {RL(af.b.h);break;}
case 0x18:  {RR(bc.b.h);break;}
case 0x19:  {RR(bc.b.l);break;}
case 0x1a:  {RR(de.b.h);break;}
case 0x1b:  {RR(de.b.l);break;}
case 0x1c:  {RR(hl.b.h);break;}
case 0x1d:  {RR(hl.b.l);break;}
case 0x1e:  {uint8 temp = m.read_byte(hl.w);RR(temp);m.write_byte(hl.w,temp);break;}
case 0x1f:  {RR(af.b.h);break;}
case 0x20:  {SLA(bc.b.h);break;}
case 0x21:  {SLA(bc.b.l);break;}
case 0x22:  {SLA(de.b.h);break;}
case 0x23:  {SLA(de.b.l);break;}
case 0x24:  {SLA(hl.b.h);break;}
case 0x25:  {SLA(hl.b.l);break;}
case 0x26:  {uint8 temp = m.read_byte(hl.w);SLA(temp);m.write_byte(hl.w,temp);break;}
case 0x27:  {SLA(af.b.h);break;}
case 0x28:  {SRA(bc.b.h);break;}
case 0x29:  {SRA(bc.b.l);break;}
case 0x2a:  {SRA(de.b.h);break;}
case 0x2b:  {SRA(de.b.l);break;}
case 0x2c:  {SRA(hl.b.h);break;}
case 0x2d:  {SRA(hl.b.l);break;}
case 0x2e:  {uint8 temp = m.read_byte(hl.w);SRA(temp);m.write_byte(hl.w,temp);break;}
case 0x2f:  {SRA(af.b.h);break;}
case 0x30:  {CB_SWAP(bc.b.h);break;}
case 0x31:  {CB_SWAP(bc.b.l);break;}
case 0x32:  {CB_SWAP(de.b.h);break;}
case 0x33:  {CB_SWAP(de.b.l);break;}
case 0x34:  {CB_SWAP(hl.b.h);break;}
case 0x35:  {CB_SWAP(hl.b.l);break;}
case 0x36:  {uint8 temp = m.read_byte(hl.w);CB_SWAP(temp);m.write_byte(hl.w,temp);break;}
case 0x37:  {CB_SWAP(af.b.h);break;}
case 0x38:  {SRL(bc.b.h);break;}
case 0x39:  {SRL(bc.b.l);break;}
case 0x3a:  {SRL(de.b.h);break;}
case 0x3b:  {SRL(de.b.l);break;}
case 0x3c:  {SRL(hl.b.h);break;}
case 0x3d:  {SRL(hl.b.l);break;}
case 0x3e:  {uint8 temp = m.read_byte(hl.w);SRL(temp);m.write_byte(hl.w,temp);break;}
case 0x3f:  {SRL(af.b.h);break;}
case 0x40:  {af.b.l = ((bc.b.h & 0x01) ? 0 : F_Z)|F_H|(F_C & af.b.l);break;}
case 0x41:  {af.b.l = ((bc.b.l & 0x01) ? 0 : F_Z)|F_H|(F_C & af.b.l);break;}
case 0x42:  {af.b.l = ((de.b.h & 0x01) ? 0 : F_Z)|F_H|(F_C & af.b.l);break;}
case 0x43:  {af.b.l = ((de.b.l & 0x01) ? 0 : F_Z)|F_H|(F_C & af.b.l);break;}
case 0x44:  {af.b.l = ((hl.b.h & 0x01) ? 0 : F_Z)|F_H|(F_C & af.b.l);break;}
case 0x45:  {af.b.l = ((hl.b.l & 0x01) ? 0 : F_Z)|F_H|(F_C & af.b.l);break;}
case 0x46:  {af.b.l = ((m.read_byte(hl.w) & 0x01) ? 0 : F_Z)|F_H|(F_C & af.b.l);break;}
case 0x47:  {af.b.l = ((af.b.h & 0x01) ? 0 : F_Z)|F_H|(F_C & af.b.l);break;}
case 0x48:  {af.b.l = ((bc.b.h & 0x02) ? 0 : F_Z)|F_H|(F_C & af.b.l);break;}
case 0x49:  {af.b.l = ((bc.b.l & 0x02) ? 0 : F_Z)|F_H|(F_C & af.b.l);break;}
case 0x4A:  {af.b.l = ((de.b.h & 0x02) ? 0 : F_Z)|F_H|(F_C & af.b.l);break;}
case 0x4B:  {af.b.l = ((de.b.l & 0x02) ? 0 : F_Z)|F_H|(F_C & af.b.l);break;}
case 0x4C:  {af.b.l = ((hl.b.h & 0x02) ? 0 : F_Z)|F_H|(F_C & af.b.l);break;}
case 0x4D:  {af.b.l = ((hl.b.l & 0x02) ? 0 : F_Z)|F_H|(F_C & af.b.l);break;}
case 0x4E:  {af.b.l = ((m.read_byte(hl.w) & 0x02) ? 0 : F_Z)|F_H|(F_C & af.b.l);break;}
case 0x4F:  {af.b.l = ((af.b.h & 0x02) ? 0 : F_Z)|F_H|(F_C & af.b.l);break;}
case 0x50:  {af.b.l = ((bc.b.h & 0x04) ? 0 : F_Z)|F_H|(F_C & af.b.l);break;}
case 0x51:  {af.b.l = ((bc.b.l & 0x04) ? 0 : F_Z)|F_H|(F_C & af.b.l);break;}
case 0x52:  {af.b.l = ((de.b.h & 0x04) ? 0 : F_Z)|F_H|(F_C & af.b.l);break;}
case 0x53:  {af.b.l = ((de.b.l & 0x04) ? 0 : F_Z)|F_H|(F_C & af.b.l);break;}
case 0x54:  {af.b.l = ((hl.b.h & 0x04) ? 0 : F_Z)|F_H|(F_C & af.b.l);break;}
case 0x55:  {af.b.l = ((hl.b.l & 0x04) ? 0 : F_Z)|F_H|(F_C & af.b.l);break;}
case 0x56:  {af.b.l = ((m.read_byte(hl.w) & 0x04) ? 0 : F_Z)|F_H|(F_C & af.b.l);break;}
case 0x57:  {af.b.l = ((af.b.h & 0x04) ? 0 : F_Z)|F_H|(F_C & af.b.l);break;}
case 0x58:  {af.b.l = ((bc.b.h & 0x08) ? 0 : F_Z)|F_H|(F_C & af.b.l);break;}
case 0x59:  {af.b.l = ((bc.b.l & 0x08) ? 0 : F_Z)|F_H|(F_C & af.b.l);break;}
case 0x5A:  {af.b.l = ((de.b.h & 0x08) ? 0 : F_Z)|F_H|(F_C & af.b.l);break;}
case 0x5B:  {af.b.l = ((de.b.l & 0x08) ? 0 : F_Z)|F_H|(F_C & af.b.l);break;}
case 0x5C:  {af.b.l = ((hl.b.h & 0x08) ? 0 : F_Z)|F_H|(F_C & af.b.l);break;}
case 0x5D:  {af.b.l = ((hl.b.l & 0x08) ? 0 : F_Z)|F_H|(F_C & af.b.l);break;}
case 0x5E:  {af.b.l = ((m.read_byte(hl.w) & 0x08) ? 0 : F_Z)|F_H|(F_C & af.b.l);break;}
case 0x5F:  {af.b.l = ((af.b.h & 0x08) ? 0 : F_Z)|F_H|(F_C & af.b.l);break;}
case 0x60:  {af.b.l = ((bc.b.h & 0x10) ? 0 : F_Z)|F_H|(F_C & af.b.l);break;}
case 0x61:  {af.b.l = ((bc.b.l & 0x10) ? 0 : F_Z)|F_H|(F_C & af.b.l);break;}
case 0x62:  {af.b.l = ((de.b.h & 0x10) ? 0 : F_Z)|F_H|(F_C & af.b.l);break;}
case 0x63:  {af.b.l = ((de.b.l & 0x10) ? 0 : F_Z)|F_H|(F_C & af.b.l);break;}
case 0x64:  {af.b.l = ((hl.b.h & 0x10) ? 0 : F_Z)|F_H|(F_C & af.b.l);break;}
case 0x65:  {af.b.l = ((hl.b.l & 0x10) ? 0 : F_Z)|F_H|(F_C & af.b.l);break;}
case 0x66:  {af.b.l = ((m.read_byte(hl.w) & 0x10) ? 0 : F_Z)|F_H|(F_C & af.b.l);break;}
case 0x67:  {af.b.l = ((af.b.h & 0x10) ? 0 : F_Z)|F_H|(F_C & af.b.l);break;}
case 0x68:  {af.b.l = ((bc.b.h & 0x20) ? 0 : F_Z)|F_H|(F_C & af.b.l);break;}
case 0x69:  {af.b.l = ((bc.b.l & 0x20) ? 0 : F_Z)|F_H|(F_C & af.b.l);break;}
case 0x6A:  {af.b.l = ((de.b.h & 0x20) ? 0 : F_Z)|F_H|(F_C & af.b.l);break;}
case 0x6B:  {af.b.l = ((de.b.l & 0x20) ? 0 : F_Z)|F_H|(F_C & af.b.l);break;}
case 0x6C:  {af.b.l = ((hl.b.h & 0x20) ? 0 : F_Z)|F_H|(F_C & af.b.l);break;}
case 0x6D:  {af.b.l = ((hl.b.l & 0x20) ? 0 : F_Z)|F_H|(F_C & af.b.l);break;}
case 0x6E:  {af.b.l = ((m.read_byte(hl.w) & 0x20) ? 0 : F_Z)|F_H|(F_C & af.b.l);break;}
case 0x6F:  {af.b.l = ((af.b.h & 0x20) ? 0 : F_Z)|F_H|(F_C & af.b.l);break;}
case 0x70:  {af.b.l = ((bc.b.h & 0x40) ? 0 : F_Z)|F_H|(F_C & af.b.l);break;}
case 0x71:  {af.b.l = ((bc.b.l & 0x40) ? 0 : F_Z)|F_H|(F_C & af.b.l);break;}
case 0x72:  {af.b.l = ((de.b.h & 0x40) ? 0 : F_Z)|F_H|(F_C & af.b.l);break;}
case 0x73:  {af.b.l = ((de.b.l & 0x40) ? 0 : F_Z)|F_H|(F_C & af.b.l);break;}
case 0x74:  {af.b.l = ((hl.b.h & 0x40) ? 0 : F_Z)|F_H|(F_C & af.b.l);break;}
case 0x75:  {af.b.l = ((hl.b.l & 0x40) ? 0 : F_Z)|F_H|(F_C & af.b.l);break;}
case 0x76:  {af.b.l = ((m.read_byte(hl.w) & 0x40) ? 0 : F_Z)|F_H|(F_C & af.b.l);break;}
case 0x77:  {af.b.l = ((af.b.h & 0x40) ? 0 : F_Z)|F_H|(F_C & af.b.l);break;}
case 0x78:  {af.b.l = ((bc.b.h & 0x80) ? 0 : F_Z)|F_H|(F_C & af.b.l);break;}
case 0x79:  {af.b.l = ((bc.b.l & 0x80) ? 0 : F_Z)|F_H|(F_C & af.b.l);break;}
case 0x7A:  {af.b.l = ((de.b.h & 0x80) ? 0 : F_Z)|F_H|(F_C & af.b.l);break;}
case 0x7B:  {af.b.l = ((de.b.l & 0x80) ? 0 : F_Z)|F_H|(F_C & af.b.l);break;}
case 0x7C:  {af.b.l = ((hl.b.h & 0x80) ? 0 : F_Z)|F_H|(F_C & af.b.l);break;}
case 0x7D:  {af.b.l = ((hl.b.l & 0x80) ? 0 : F_Z)|F_H|(F_C & af.b.l);break;}
case 0x7E:  {af.b.l = ((m.read_byte(hl.w) & 0x80) ? 0 : F_Z)|F_H|(F_C & af.b.l);break;}
case 0x7F:  {af.b.l = ((af.b.h & 0x80) ? 0 : F_Z)|F_H|(F_C & af.b.l);break;}
case 0x80:  {CLEAR(0x1,bc.b.h);break;}
case 0x81:  {CLEAR(0x1,bc.b.l);break;}
case 0x82:  {CLEAR(0x1,de.b.h);break;}
case 0x83:  {CLEAR(0x1,de.b.l);break;}
case 0x84:  {CLEAR(0x1,hl.b.h);break;}
case 0x85:  {CLEAR(0x1,hl.b.l);break;}
case 0x86:  {uint8 temp = m.read_byte(hl.w);CLEAR(0x1,temp);m.write_byte(hl.w,temp);break;}
case 0x87:  {CLEAR(0x1,af.b.h);break;}
case 0x88:  {CLEAR(0x2,bc.b.h);break;}
case 0x89:  {CLEAR(0x2,bc.b.l);break;}
case 0x8A:  {CLEAR(0x2,de.b.h);break;}
case 0x8B:  {CLEAR(0x2,de.b.l);break;}
case 0x8C:  {CLEAR(0x2,hl.b.h);break;}
case 0x8D:  {CLEAR(0x2,hl.b.l);break;}
case 0x8E:  {uint8 temp = m.read_byte(hl.w);CLEAR(0x2,temp);m.write_byte(hl.w,temp);break;}
case 0x8F:  {CLEAR(0x2,af.b.h);break;}
case 0x90:  {CLEAR(0x4,bc.b.h);break;}
case 0x91:  {CLEAR(0x4,bc.b.l);break;}
case 0x92:  {CLEAR(0x4,de.b.h);break;}
case 0x93:  {CLEAR(0x4,de.b.l);break;}
case 0x94:  {CLEAR(0x4,hl.b.h);break;}
case 0x95:  {CLEAR(0x4,hl.b.l);break;}
case 0x96:  {uint8 temp = m.read_byte(hl.w);CLEAR(0x4,temp);m.write_byte(hl.w,temp);break;}
case 0x97:  {CLEAR(0x4,af.b.h);break;}
case 0x98:  {CLEAR(0x8,bc.b.h);break;}
case 0x99:  {CLEAR(0x8,bc.b.l);break;}
case 0x9A:  {CLEAR(0x8,de.b.h);break;}
case 0x9B:  {CLEAR(0x8,de.b.l);break;}
case 0x9C:  {CLEAR(0x8,hl.b.h);break;}
case 0x9D:  {CLEAR(0x8,hl.b.l);break;}
case 0x9E:  {uint8 temp = m.read_byte(hl.w);CLEAR(0x8,temp);m.write_byte(hl.w,temp);break;}
case 0x9F:  {CLEAR(0x8,af.b.h);break;}
case 0xa0:  {CLEAR(0x10,bc.b.h);break;}
case 0xa1:  {CLEAR(0x10,bc.b.l);break;}
case 0xa2:  {CLEAR(0x10,de.b.h);break;}
case 0xa3:  {CLEAR(0x10,de.b.l);break;}
case 0xa4:  {CLEAR(0x10,hl.b.h);break;}
case 0xa5:  {CLEAR(0x10,hl.b.l);break;}
case 0xa6:  {uint8 temp = m.read_byte(hl.w);CLEAR(0x10,temp);m.write_byte(hl.w,temp);break;}
case 0xa7:  {CLEAR(0x10,af.b.h);break;}
case 0xa8:  {CLEAR(0x20,bc.b.h);break;}
case 0xa9:  {CLEAR(0x20,bc.b.l);break;}
case 0xaA:  {CLEAR(0x20,de.b.h);break;}
case 0xaB:  {CLEAR(0x20,de.b.l);break;}
case 0xaC:  {CLEAR(0x20,hl.b.h);break;}
case 0xaD:  {CLEAR(0x20,hl.b.l);break;}
case 0xaE:  {uint8 temp = m.read_byte(hl.w);CLEAR(0x20,temp);m.write_byte(hl.w,temp);break;}
case 0xaF:  {CLEAR(0x20,af.b.h);break;}
case 0xb0:  {CLEAR(0x40,bc.b.h);break;}
case 0xb1:  {CLEAR(0x40,bc.b.l);break;}
case 0xb2:  {CLEAR(0x40,de.b.h);break;}
case 0xb3:  {CLEAR(0x40,de.b.l);break;}
case 0xb4:  {CLEAR(0x40,hl.b.h);break;}
case 0xb5:  {CLEAR(0x40,hl.b.l);break;}
case 0xb6:  {uint8 temp = m.read_byte(hl.w);CLEAR(0x40,temp);m.write_byte(hl.w,temp);break;}
case 0xb7:  {CLEAR(0x40,af.b.h);break;}
case 0xb8:  {CLEAR(0x80,bc.b.h);break;}
case 0xb9:  {CLEAR(0x80,bc.b.l);break;}
case 0xbA:  {CLEAR(0x80,de.b.h);break;}
case 0xbB:  {CLEAR(0x80,de.b.l);break;}
case 0xbC:  {CLEAR(0x80,hl.b.h);break;}
case 0xbD:  {CLEAR(0x80,hl.b.l);break;}
case 0xbE:  {uint8 temp = m.read_byte(hl.w);CLEAR(0x80,temp);m.write_byte(hl.w,temp);break;}
case 0xbF:  {CLEAR(0x80,af.b.h);break;}
case 0xc0:  {SET(0x1,bc.b.h);break;}
case 0xc1:  {SET(0x1,bc.b.l);break;}
case 0xc2:  {SET(0x1,de.b.h);break;}
case 0xc3:  {SET(0x1,de.b.l);break;}
case 0xc4:  {SET(0x1,hl.b.h);break;}
case 0xc5:  {SET(0x1,hl.b.l);break;}
case 0xc6:  {uint8 temp = m.read_byte(hl.w);SET(0x1,temp);m.write_byte(hl.w,temp);break;}
case 0xc7:  {SET(0x1,af.b.h);break;}
case 0xc8:  {SET(0x2,bc.b.h);break;}
case 0xc9:  {SET(0x2,bc.b.l);break;}
case 0xcA:  {SET(0x2,de.b.h);break;}
case 0xcB:  {SET(0x2,de.b.l);break;}
case 0xcC:  {SET(0x2,hl.b.h);break;}
case 0xcD:  {SET(0x2,hl.b.l);break;}
case 0xcE:  {uint8 temp = m.read_byte(hl.w);SET(0x2,temp);m.write_byte(hl.w,temp);break;}
case 0xcF:  {SET(0x2,af.b.h);break;}
case 0xd0:  {SET(0x4,bc.b.h);break;}
case 0xd1:  {SET(0x4,bc.b.l);break;}
case 0xd2:  {SET(0x4,de.b.h);break;}
case 0xd3:  {SET(0x4,de.b.l);break;}
case 0xd4:  {SET(0x4,hl.b.h);break;}
case 0xd5:  {SET(0x4,hl.b.l);break;}
case 0xd6:  {uint8 temp = m.read_byte(hl.w);SET(0x4,temp);m.write_byte(hl.w,temp);break;}
case 0xd7:  {SET(0x4,af.b.h);break;}
case 0xd8:  {SET(0x8,bc.b.h);break;}
case 0xd9:  {SET(0x8,bc.b.l);break;}
case 0xdA:  {SET(0x8,de.b.h);break;}
case 0xdB:  {SET(0x8,de.b.l);break;}
case 0xdC:  {SET(0x8,hl.b.h);break;}
case 0xdD:  {SET(0x8,hl.b.l);break;}
case 0xdE:  {uint8 temp = m.read_byte(hl.w);SET(0x8,temp);m.write_byte(hl.w,temp);break;}
case 0xdF:  {SET(0x8,af.b.h);break;}
case 0xe0:  {SET(0x10,bc.b.h);break;}
case 0xe1:  {SET(0x10,bc.b.l);break;}
case 0xe2:  {SET(0x10,de.b.h);break;}
case 0xe3:  {SET(0x10,de.b.l);break;}
case 0xe4:  {SET(0x10,hl.b.h);break;}
case 0xe5:  {SET(0x10,hl.b.l);break;}
case 0xe6:  {uint8 temp = m.read_byte(hl.w);SET(0x10,temp);m.write_byte(hl.w,temp);break;}
case 0xe7:  {SET(0x10,af.b.h);break;}
case 0xe8:  {SET(0x20,bc.b.h);break;}
case 0xe9:  {SET(0x20,bc.b.l);break;}
case 0xeA:  {SET(0x20,de.b.h);break;}
case 0xeB:  {SET(0x20,de.b.l);break;}
case 0xeC:  {SET(0x20,hl.b.h);break;}
case 0xeD:  {SET(0x20,hl.b.l);break;}
case 0xeE:  {uint8 temp = m.read_byte(hl.w);SET(0x20,temp);m.write_byte(hl.w,temp);break;}
case 0xeF:  {SET(0x20,af.b.h);break;}
case 0xf0:  {SET(0x40,bc.b.h);break;}
case 0xf1:  {SET(0x40,bc.b.l);break;}
case 0xf2:  {SET(0x40,de.b.h);break;}
case 0xf3:  {SET(0x40,de.b.l);break;}
case 0xf4:  {SET(0x40,hl.b.h);break;}
case 0xf5:  {SET(0x40,hl.b.l);break;}
case 0xf6:  {uint8 temp = m.read_byte(hl.w);SET(0x40,temp);m.write_byte(hl.w,temp);break;}
case 0xf7:  {SET(0x40,af.b.h);break;}
case 0xf8:  {SET(0x80,bc.b.h);break;}
case 0xf9:  {SET(0x80,bc.b.l);break;}
case 0xfA:  {SET(0x80,de.b.h);break;}
case 0xfB:  {SET(0x80,de.b.l);break;}
case 0xfC:  {SET(0x80,hl.b.h);break;}
case 0xfD:  {SET(0x80,hl.b.l);break;}
case 0xfE:  {uint8 temp = m.read_byte(hl.w);SET(0x80,temp);m.write_byte(hl.w,temp);break;}
case 0xfF:  {SET(0x80,af.b.h);break;}
default:  {printf("cb opcode 0x%x not found\n",op);getchar();}
