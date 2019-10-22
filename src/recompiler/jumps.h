//JR
case 0x18: {JIT_JR(m.read_byte(pc.w + 1));break;}
//JP
case 0xc3: {JIT_JP(m.read_word(pc.w + 1));break;}
//RST 0x00
case 0xc7: {JIT_RST(0x00);break;}
//RET
case 0xc9: {JIT_RET;break;}
//CALL
case 0xcd: {JIT_CALL(m.read_word(pc.w + 1));break;}
//RST 0x08
case 0xcf: {JIT_RST(0x08);break;}
//RST 0x10
case 0xd7: {JIT_RST(0x10);break;}
//RETI
case 0xd9: {JIT_RETI;break;}
//RST 0x18
case 0xdf: {JIT_RST(0x18);break;}
//RST 0x20
case 0xe7: {JIT_RST(0x20);break;}
//JP
case 0xe9: {JIT_JP(hl.w);break;}
//RST 0x28
case 0xef: {JIT_RST(0x28);break;}
//RST 0x30
case 0xf7: {JIT_RST(0x30);break;}
//RST 0x38
case 0xff: {JIT_RST(0x38);break;}
