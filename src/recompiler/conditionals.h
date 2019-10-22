case 0x20: {JIT_COND_JR(!(af.b.l & F_Z),m.read_byte(pc.w + 1));break;}
case 0x28: {JIT_COND_JR((af.b.l & F_Z),m.read_byte(pc.w + 1));break;}
case 0x30: {JIT_COND_JR(!(af.b.l & F_C),m.read_byte(pc.w + 1));break;}
case 0x38: {JIT_COND_JR((af.b.l & F_C),m.read_byte(pc.w + 1));break;}
case 0xc0: {JIT_COND_RET(!(af.b.l & F_Z));break;}
case 0xc2: {JIT_COND_JP(!(af.b.l & F_Z),m.read_word(pc.w + 1));break;}
case 0xc4: {JIT_COND_CALL(!(af.b.l & F_Z),m.read_word(pc.w + 1));break;}
case 0xc8: {JIT_COND_RET((af.b.l & F_Z));break;}
case 0xca: {JIT_COND_JP((af.b.l & F_Z),m.read_word(pc.w + 1));break;}
case 0xcc: {JIT_COND_CALL((af.b.l & F_Z),m.read_word(pc.w + 1));break;}
case 0xd0: {JIT_COND_RET(!(af.b.l & F_C));break;}
case 0xd2: {JIT_COND_JP(!(af.b.l & F_C),m.read_word(pc.w + 1));break;}
case 0xd4: {JIT_COND_CALL(!(af.b.l & F_C),m.read_word(pc.w + 1));break;}
case 0xd8: {JIT_COND_RET((af.b.l & F_C));break;}
case 0xda: {JIT_COND_JP((af.b.l & F_C),m.read_word(pc.w + 1));break;}
case 0xdc: {JIT_COND_CALL((af.b.l & F_C),m.read_word(pc.w + 1));break;}
