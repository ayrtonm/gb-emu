case 0x20: {COND_JR(!(af.b.l & F_Z),m.read_byte(pc.w + 1));break;}
case 0x28: {COND_JR((af.b.l & F_Z),m.read_byte(pc.w + 1));break;}
case 0x30: {COND_JR(!(af.b.l & F_C),m.read_byte(pc.w + 1));break;}
case 0x38: {COND_JR((af.b.l & F_C),m.read_byte(pc.w + 1));break;}
case 0xc0: {COND_RET(!(af.b.l & F_Z));break;}
case 0xc2: {COND_JP(!(af.b.l & F_Z),m.read_word(pc.w + 1));break;}
case 0xc4: {COND_CALL(!(af.b.l & F_Z),m.read_word(pc.w + 1));break;}
case 0xc8: {COND_RET((af.b.l & F_Z));break;}
case 0xca: {COND_JP((af.b.l & F_Z),m.read_word(pc.w + 1));break;}
case 0xcc: {COND_CALL((af.b.l & F_Z),m.read_word(pc.w + 1));break;}
case 0xd0: {COND_RET(!(af.b.l & F_C));break;}
case 0xd2: {COND_JP(!(af.b.l & F_C),m.read_word(pc.w + 1));break;}
case 0xd4: {COND_CALL(!(af.b.l & F_C),m.read_word(pc.w + 1));break;}
case 0xd8: {COND_RET((af.b.l & F_C));break;}
case 0xda: {COND_JP((af.b.l & F_C),m.read_word(pc.w + 1));break;}
case 0xdc: {COND_CALL((af.b.l & F_C),m.read_word(pc.w + 1));break;}
