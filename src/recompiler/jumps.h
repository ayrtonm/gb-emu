//JR
case 0x18: {JR(m.read_byte(pc.w + 1));break;}
//JP
case 0xc3: {JP(m.read_word(pc.w + 1));break;}
//RST 0x00
case 0xc7: {pc.w += 3;RST(0x00);break;}
//RET
case 0xc9: {RET;break;}
//CALL
case 0xcd: {pc.w += 3;CALL(m.read_word(pc.w - 2));break;}
//RST 0x08
case 0xcf: {pc.w += 3;RST(0x08);break;}
//RST 0x10
case 0xd7: {pc.w += 3;RST(0x10);break;}
//RETI
case 0xd9: {RETI;break;}
//RST 0x18
case 0xdf: {pc.w += 3;RST(0x18);break;}
//RST 0x20
case 0xe7: {pc.w += 3;RST(0x20);break;}
//JP
case 0xe9: {JP(hl.w);break;}
//RST 0x28
case 0xef: {pc.w += 3;RST(0x28);break;}
//RST 0x30
case 0xf7: {pc.w += 3;RST(0x30);break;}
//RST 0x38
case 0xff: {pc.w += 3;RST(0x38);break;}
