#ifndef CPU_H
#define CPU_H
#include "../bits.h"
#include "../mem.h"
#include "../lcd.h"
#include "../keypad.h"
#include "../sound.h"

class cpu {
  public:
    cpu();
    word16 af, bc, de, hl, sp, pc;
    uint8 ime;
    uint8 ei_delay;
    uint8 halt;
    bool repeat;
    struct timespec wait;
    double sleep_factor[4];
    int cputhrottleclk;
    int emulate(mem &m, keypad &k);
    void throttle(int dt);
};

//opcodes
#define INC(r) do {r++; af.b.l = (af.b.l & F_C)|((r) ? 0 : F_Z)|((r) & 0x0F ? 0 : F_H);} while(0)

#define DEC(r) do {r--; af.b.l = F_N|(af.b.l & F_C)|((r) ? 0 : F_Z)|(((r) & 0x0F)==0x0F ? F_H : 0);} while(0)

#define ADDHL(r) do {int mtemp = hl.w+(r); af.b.l = (af.b.l & F_Z)|(mtemp & 0x010000 ? F_C : 0)|((hl.w^(r)^(mtemp & 0xFFFF)) & 0x1000 ? F_H : 0); hl.w = mtemp & 0xFFFF;} while(0)

#define JR(n) do {pc.w += ((signed char)n);} while(0)

#define COND_JR(cond,n) do {if (cond) {JR(n);dt += 1;};} while(0)

#define RET do {POP(pc.b.h,pc.b.l);} while(0)

#define COND_RET(cond) do {if (cond) {RET;dt += 3;};} while(0)

#define RETI do {POP(pc.b.h,pc.b.l);ime=1;} while(0)

#define JP(n) do {pc.w = n;} while(0)

#define COND_JP(cond,n) do {if (cond) {JP(n); dt += 1;};} while(0)

#define CALL(n) do {PUSH(pc.b.h,pc.b.l); pc.w = n;} while(0)

#define COND_CALL(cond,n) do {if (cond) {CALL(n);dt += 3;};} while(0)

#define POP(a,b) do {b = m.read_byte(sp.w++); a = m.read_byte(sp.w++);} while(0)

#define PUSH(a,b) do {m.write_byte(--sp.w,a); m.write_byte(--sp.w,b);} while(0)

#define RST(n) do {PUSH(pc.b.h,pc.b.l); pc.w = n;} while(0)

#define DAA do {uint16 mtemp = af.b.h; mtemp |= (af.b.l & (F_C|F_H|F_N)) << 4; af.w = DAATable[mtemp];} while(0)

#define CPL do {af.b.h ^= 0xFF; af.b.l |= (F_N|F_H);} while(0)

#define SCL do {af.b.l = (af.b.l & F_Z) | F_C;} while(0)

#define CCF do {af.b.l ^= F_C; af.b.l &= ~(F_N|F_H);} while(0)

#define LD(x,y) do {x=y;} while(0)

#define LD_MR(x,y) do {x=m.read_byte(y);} while(0)

#define LD_RM(x,y) do {m.write_byte(x,y);} while(0)

#define ADD(r) do {uint16 mtemp = af.b.h + (r); af.b.l = ((mtemp & 0xFF00) ? F_C : 0)|((mtemp & 0x00FF) ? 0 : F_Z)|((af.b.h^(r)^(mtemp & 0x00FF)) & 0x10 ? F_H : 0); af.b.h = mtemp & 0x00FF;} while(0)

#define ADC(r) do {uint16 mtemp = af.b.h + (r) + ((af.b.l & F_C) ? 1 : 0); af.b.l = ((mtemp & 0xFF00) ? F_C : 0)|((mtemp & 0x00FF) ? 0 : F_Z)|((af.b.h^(r)^(mtemp & 0x00FF)) & 0x10 ? F_H : 0); af.b.h = mtemp & 0x00FF;} while(0)

#define SUB(r) do {uint16 mtemp = af.b.h - (r); af.b.l = F_N|((mtemp & 0xFF00) ? F_C : 0)|((mtemp & 0x00FF) ? 0 : F_Z)|((af.b.h^(r)^(mtemp & 0x00FF)) & 0x10 ? F_H : 0); af.b.h = mtemp & 0x00FF;} while(0)

#define SBC(r) do {uint16 mtemp = af.b.h - (r) - ((af.b.l & F_C) ? 1 : 0); af.b.l = F_N|((mtemp & 0xFF00) ? F_C : 0)|((mtemp & 0x00FF) ? 0 : F_Z)|((af.b.h^(r)^(mtemp & 0x00FF)) & 0x10 ? F_H : 0); af.b.h = mtemp & 0x00FF;} while(0)

#define AND(r) do {af.b.h &= (r); af.b.l = F_H|(af.b.h > 0 ? 0 : F_Z);} while(0)

#define EOR(r) do {af.b.h ^= (r); af.b.l = (af.b.h ? 0 : F_Z);} while(0)

#define OR(r) do {af.b.h |= (r); af.b.l = (af.b.h ? 0 : F_Z);} while(0)

#define CP(r) do {uint16 mtemp = af.b.h - (r); af.b.l = F_N|((mtemp & 0xFF00) ? F_C : 0)|((mtemp & 0x00FF) ? 0 : F_Z)|((af.b.h^(r)^(mtemp & 0x00FF)) & 0x10 ? F_H : 0);} while(0)

//cb opcodes
#define RLCA do {uint8 mtemp = (af.b.h & 0x80 ? F_C : 0); af.b.h = (af.b.h << 1)|(af.b.h >> 7); af.b.l = mtemp;} while(0)

#define RRCA do {uint8 mtemp = af.b.h & 0x01; af.b.h = (af.b.h >> 1)|(mtemp ? 0x80 : 0); af.b.l = (mtemp << 4);} while(0)

#define RLA do {uint8 mtemp = (af.b.h & 0x80 ? F_C : 0); af.b.h = (af.b.h << 1)|((af.b.l & F_C) >> 4); af.b.l = mtemp;} while(0)

#define RRA do {uint8 mtemp = af.b.h & 0x01; af.b.h = (af.b.h >> 1)|(af.b.l & F_C ? 0x80 : 0); af.b.l = (mtemp << 4);} while(0)

#define RLC(r) do {af.b.l = ((r) & 0x80) ? F_C : 0; r = (r << 1) | (r >> 7); af.b.l |= (r ? 0 : F_Z);} while(0)

#define RRC(r) do {af.b.l = ((r) & 0x01) ? F_C : 0; r = (r >> 1) | (r << 7); af.b.l |= (r ? 0 : F_Z);} while(0)

#define RL(r) do {if (r & 0x80) {r = (r << 1) | (af.b.l & F_C ? 1 : 0); af.b.l = (r ? 0 : F_Z)|F_C;} else {r = (r << 1) | (af.b.l & F_C ? 1 : 0); af.b.l = (r ? 0 : F_Z);};} while(0)

#define RR(r) do {if (r & 0x01) {r = (r >> 1) | (af.b.l & F_C ? 0x80 : 0); af.b.l = (r ? 0 : F_Z)|F_C;} else {r = (r >> 1) | (af.b.l & F_C ? 0x80 : 0); af.b.l = (r ? 0 : F_Z);};} while(0)

#define SLA(r) do {af.b.l = (r & 0x80) ? F_C : 0; r <<= 1; af.b.l |= (r ? 0 : F_Z);} while(0)

#define SRA(r) do {af.b.l = (r & 0x01) ? F_C : 0; r = (r >> 1) | (r & 0x80); af.b.l |= (r ? 0 : F_Z);} while(0)

#define CB_SWAP(r) do {r = SWAP(r); af.b.l = (r ? 0 : F_Z);} while(0)

#define SRL(r) do {af.b.l = (r & 0x01) ? F_C : 0; r >>= 1; af.b.l |= (r ? 0 : F_Z);} while(0)

static const uint16 DAATable[] = {
  0x0080,0x0100,0x0200,0x0300,0x0400,0x0500,0x0600,0x0700,
  0x0800,0x0900,0x1000,0x1100,0x1200,0x1300,0x1400,0x1500,
  0x1000,0x1100,0x1200,0x1300,0x1400,0x1500,0x1600,0x1700,
  0x1800,0x1900,0x2000,0x2100,0x2200,0x2300,0x2400,0x2500,
  0x2000,0x2100,0x2200,0x2300,0x2400,0x2500,0x2600,0x2700,
  0x2800,0x2900,0x3000,0x3100,0x3200,0x3300,0x3400,0x3500,
  0x3000,0x3100,0x3200,0x3300,0x3400,0x3500,0x3600,0x3700,
  0x3800,0x3900,0x4000,0x4100,0x4200,0x4300,0x4400,0x4500,
  0x4000,0x4100,0x4200,0x4300,0x4400,0x4500,0x4600,0x4700,
  0x4800,0x4900,0x5000,0x5100,0x5200,0x5300,0x5400,0x5500,
  0x5000,0x5100,0x5200,0x5300,0x5400,0x5500,0x5600,0x5700,
  0x5800,0x5900,0x6000,0x6100,0x6200,0x6300,0x6400,0x6500,
  0x6000,0x6100,0x6200,0x6300,0x6400,0x6500,0x6600,0x6700,
  0x6800,0x6900,0x7000,0x7100,0x7200,0x7300,0x7400,0x7500,
  0x7000,0x7100,0x7200,0x7300,0x7400,0x7500,0x7600,0x7700,
  0x7800,0x7900,0x8000,0x8100,0x8200,0x8300,0x8400,0x8500,
  0x8000,0x8100,0x8200,0x8300,0x8400,0x8500,0x8600,0x8700,
  0x8800,0x8900,0x9000,0x9100,0x9200,0x9300,0x9400,0x9500,
  0x9000,0x9100,0x9200,0x9300,0x9400,0x9500,0x9600,0x9700,
  0x9800,0x9900,0x0090,0x0110,0x0210,0x0310,0x0410,0x0510,
  0x0090,0x0110,0x0210,0x0310,0x0410,0x0510,0x0610,0x0710,
  0x0810,0x0910,0x1010,0x1110,0x1210,0x1310,0x1410,0x1510,
  0x1010,0x1110,0x1210,0x1310,0x1410,0x1510,0x1610,0x1710,
  0x1810,0x1910,0x2010,0x2110,0x2210,0x2310,0x2410,0x2510,
  0x2010,0x2110,0x2210,0x2310,0x2410,0x2510,0x2610,0x2710,
  0x2810,0x2910,0x3010,0x3110,0x3210,0x3310,0x3410,0x3510,
  0x3010,0x3110,0x3210,0x3310,0x3410,0x3510,0x3610,0x3710,
  0x3810,0x3910,0x4010,0x4110,0x4210,0x4310,0x4410,0x4510,
  0x4010,0x4110,0x4210,0x4310,0x4410,0x4510,0x4610,0x4710,
  0x4810,0x4910,0x5010,0x5110,0x5210,0x5310,0x5410,0x5510,
  0x5010,0x5110,0x5210,0x5310,0x5410,0x5510,0x5610,0x5710,
  0x5810,0x5910,0x6010,0x6110,0x6210,0x6310,0x6410,0x6510,
  0x6010,0x6110,0x6210,0x6310,0x6410,0x6510,0x6610,0x6710,
  0x6810,0x6910,0x7010,0x7110,0x7210,0x7310,0x7410,0x7510,
  0x7010,0x7110,0x7210,0x7310,0x7410,0x7510,0x7610,0x7710,
  0x7810,0x7910,0x8010,0x8110,0x8210,0x8310,0x8410,0x8510,
  0x8010,0x8110,0x8210,0x8310,0x8410,0x8510,0x8610,0x8710,
  0x8810,0x8910,0x9010,0x9110,0x9210,0x9310,0x9410,0x9510,
  0x9010,0x9110,0x9210,0x9310,0x9410,0x9510,0x9610,0x9710,
  0x9810,0x9910,0xA010,0xA110,0xA210,0xA310,0xA410,0xA510,
  0xA010,0xA110,0xA210,0xA310,0xA410,0xA510,0xA610,0xA710,
  0xA810,0xA910,0xB010,0xB110,0xB210,0xB310,0xB410,0xB510,
  0xB010,0xB110,0xB210,0xB310,0xB410,0xB510,0xB610,0xB710,
  0xB810,0xB910,0xC010,0xC110,0xC210,0xC310,0xC410,0xC510,
  0xC010,0xC110,0xC210,0xC310,0xC410,0xC510,0xC610,0xC710,
  0xC810,0xC910,0xD010,0xD110,0xD210,0xD310,0xD410,0xD510,
  0xD010,0xD110,0xD210,0xD310,0xD410,0xD510,0xD610,0xD710,
  0xD810,0xD910,0xE010,0xE110,0xE210,0xE310,0xE410,0xE510,
  0xE010,0xE110,0xE210,0xE310,0xE410,0xE510,0xE610,0xE710,
  0xE810,0xE910,0xF010,0xF110,0xF210,0xF310,0xF410,0xF510,
  0xF010,0xF110,0xF210,0xF310,0xF410,0xF510,0xF610,0xF710,
  0xF810,0xF910,0x0090,0x0110,0x0210,0x0310,0x0410,0x0510,
  0x0090,0x0110,0x0210,0x0310,0x0410,0x0510,0x0610,0x0710,
  0x0810,0x0910,0x1010,0x1110,0x1210,0x1310,0x1410,0x1510,
  0x1010,0x1110,0x1210,0x1310,0x1410,0x1510,0x1610,0x1710,
  0x1810,0x1910,0x2010,0x2110,0x2210,0x2310,0x2410,0x2510,
  0x2010,0x2110,0x2210,0x2310,0x2410,0x2510,0x2610,0x2710,
  0x2810,0x2910,0x3010,0x3110,0x3210,0x3310,0x3410,0x3510,
  0x3010,0x3110,0x3210,0x3310,0x3410,0x3510,0x3610,0x3710,
  0x3810,0x3910,0x4010,0x4110,0x4210,0x4310,0x4410,0x4510,
  0x4010,0x4110,0x4210,0x4310,0x4410,0x4510,0x4610,0x4710,
  0x4810,0x4910,0x5010,0x5110,0x5210,0x5310,0x5410,0x5510,
  0x5010,0x5110,0x5210,0x5310,0x5410,0x5510,0x5610,0x5710,
  0x5810,0x5910,0x6010,0x6110,0x6210,0x6310,0x6410,0x6510,
  0x0600,0x0700,0x0800,0x0900,0x0A00,0x0B00,0x0C00,0x0D00,
  0x0E00,0x0F00,0x1000,0x1100,0x1200,0x1300,0x1400,0x1500,
  0x1600,0x1700,0x1800,0x1900,0x1A00,0x1B00,0x1C00,0x1D00,
  0x1E00,0x1F00,0x2000,0x2100,0x2200,0x2300,0x2400,0x2500,
  0x2600,0x2700,0x2800,0x2900,0x2A00,0x2B00,0x2C00,0x2D00,
  0x2E00,0x2F00,0x3000,0x3100,0x3200,0x3300,0x3400,0x3500,
  0x3600,0x3700,0x3800,0x3900,0x3A00,0x3B00,0x3C00,0x3D00,
  0x3E00,0x3F00,0x4000,0x4100,0x4200,0x4300,0x4400,0x4500,
  0x4600,0x4700,0x4800,0x4900,0x4A00,0x4B00,0x4C00,0x4D00,
  0x4E00,0x4F00,0x5000,0x5100,0x5200,0x5300,0x5400,0x5500,
  0x5600,0x5700,0x5800,0x5900,0x5A00,0x5B00,0x5C00,0x5D00,
  0x5E00,0x5F00,0x6000,0x6100,0x6200,0x6300,0x6400,0x6500,
  0x6600,0x6700,0x6800,0x6900,0x6A00,0x6B00,0x6C00,0x6D00,
  0x6E00,0x6F00,0x7000,0x7100,0x7200,0x7300,0x7400,0x7500,
  0x7600,0x7700,0x7800,0x7900,0x7A00,0x7B00,0x7C00,0x7D00,
  0x7E00,0x7F00,0x8000,0x8100,0x8200,0x8300,0x8400,0x8500,
  0x8600,0x8700,0x8800,0x8900,0x8A00,0x8B00,0x8C00,0x8D00,
  0x8E00,0x8F00,0x9000,0x9100,0x9200,0x9300,0x9400,0x9500,
  0x9600,0x9700,0x9800,0x9900,0x9A00,0x9B00,0x9C00,0x9D00,
  0x9E00,0x9F00,0x0090,0x0110,0x0210,0x0310,0x0410,0x0510,
  0x0610,0x0710,0x0810,0x0910,0x0A10,0x0B10,0x0C10,0x0D10,
  0x0E10,0x0F10,0x1010,0x1110,0x1210,0x1310,0x1410,0x1510,
  0x1610,0x1710,0x1810,0x1910,0x1A10,0x1B10,0x1C10,0x1D10,
  0x1E10,0x1F10,0x2010,0x2110,0x2210,0x2310,0x2410,0x2510,
  0x2610,0x2710,0x2810,0x2910,0x2A10,0x2B10,0x2C10,0x2D10,
  0x2E10,0x2F10,0x3010,0x3110,0x3210,0x3310,0x3410,0x3510,
  0x3610,0x3710,0x3810,0x3910,0x3A10,0x3B10,0x3C10,0x3D10,
  0x3E10,0x3F10,0x4010,0x4110,0x4210,0x4310,0x4410,0x4510,
  0x4610,0x4710,0x4810,0x4910,0x4A10,0x4B10,0x4C10,0x4D10,
  0x4E10,0x4F10,0x5010,0x5110,0x5210,0x5310,0x5410,0x5510,
  0x5610,0x5710,0x5810,0x5910,0x5A10,0x5B10,0x5C10,0x5D10,
  0x5E10,0x5F10,0x6010,0x6110,0x6210,0x6310,0x6410,0x6510,
  0x6610,0x6710,0x6810,0x6910,0x6A10,0x6B10,0x6C10,0x6D10,
  0x6E10,0x6F10,0x7010,0x7110,0x7210,0x7310,0x7410,0x7510,
  0x7610,0x7710,0x7810,0x7910,0x7A10,0x7B10,0x7C10,0x7D10,
  0x7E10,0x7F10,0x8010,0x8110,0x8210,0x8310,0x8410,0x8510,
  0x8610,0x8710,0x8810,0x8910,0x8A10,0x8B10,0x8C10,0x8D10,
  0x8E10,0x8F10,0x9010,0x9110,0x9210,0x9310,0x9410,0x9510,
  0x9610,0x9710,0x9810,0x9910,0x9A10,0x9B10,0x9C10,0x9D10,
  0x9E10,0x9F10,0xA010,0xA110,0xA210,0xA310,0xA410,0xA510,
  0xA610,0xA710,0xA810,0xA910,0xAA10,0xAB10,0xAC10,0xAD10,
  0xAE10,0xAF10,0xB010,0xB110,0xB210,0xB310,0xB410,0xB510,
  0xB610,0xB710,0xB810,0xB910,0xBA10,0xBB10,0xBC10,0xBD10,
  0xBE10,0xBF10,0xC010,0xC110,0xC210,0xC310,0xC410,0xC510,
  0xC610,0xC710,0xC810,0xC910,0xCA10,0xCB10,0xCC10,0xCD10,
  0xCE10,0xCF10,0xD010,0xD110,0xD210,0xD310,0xD410,0xD510,
  0xD610,0xD710,0xD810,0xD910,0xDA10,0xDB10,0xDC10,0xDD10,
  0xDE10,0xDF10,0xE010,0xE110,0xE210,0xE310,0xE410,0xE510,
  0xE610,0xE710,0xE810,0xE910,0xEA10,0xEB10,0xEC10,0xED10,
  0xEE10,0xEF10,0xF010,0xF110,0xF210,0xF310,0xF410,0xF510,
  0xF610,0xF710,0xF810,0xF910,0xFA10,0xFB10,0xFC10,0xFD10,
  0xFE10,0xFF10,0x0090,0x0110,0x0210,0x0310,0x0410,0x0510,
  0x0610,0x0710,0x0810,0x0910,0x0A10,0x0B10,0x0C10,0x0D10,
  0x0E10,0x0F10,0x1010,0x1110,0x1210,0x1310,0x1410,0x1510,
  0x1610,0x1710,0x1810,0x1910,0x1A10,0x1B10,0x1C10,0x1D10,
  0x1E10,0x1F10,0x2010,0x2110,0x2210,0x2310,0x2410,0x2510,
  0x2610,0x2710,0x2810,0x2910,0x2A10,0x2B10,0x2C10,0x2D10,
  0x2E10,0x2F10,0x3010,0x3110,0x3210,0x3310,0x3410,0x3510,
  0x3610,0x3710,0x3810,0x3910,0x3A10,0x3B10,0x3C10,0x3D10,
  0x3E10,0x3F10,0x4010,0x4110,0x4210,0x4310,0x4410,0x4510,
  0x4610,0x4710,0x4810,0x4910,0x4A10,0x4B10,0x4C10,0x4D10,
  0x4E10,0x4F10,0x5010,0x5110,0x5210,0x5310,0x5410,0x5510,
  0x5610,0x5710,0x5810,0x5910,0x5A10,0x5B10,0x5C10,0x5D10,
  0x5E10,0x5F10,0x6010,0x6110,0x6210,0x6310,0x6410,0x6510,
  0x00C0,0x0140,0x0240,0x0340,0x0440,0x0540,0x0640,0x0740,
  0x0840,0x0940,0x0A40,0x0B40,0x0C40,0x0D40,0x0E40,0x0F40,
  0x1040,0x1140,0x1240,0x1340,0x1440,0x1540,0x1640,0x1740,
  0x1840,0x1940,0x1A40,0x1B40,0x1C40,0x1D40,0x1E40,0x1F40,
  0x2040,0x2140,0x2240,0x2340,0x2440,0x2540,0x2640,0x2740,
  0x2840,0x2940,0x2A40,0x2B40,0x2C40,0x2D40,0x2E40,0x2F40,
  0x3040,0x3140,0x3240,0x3340,0x3440,0x3540,0x3640,0x3740,
  0x3840,0x3940,0x3A40,0x3B40,0x3C40,0x3D40,0x3E40,0x3F40,
  0x4040,0x4140,0x4240,0x4340,0x4440,0x4540,0x4640,0x4740,
  0x4840,0x4940,0x4A40,0x4B40,0x4C40,0x4D40,0x4E40,0x4F40,
  0x5040,0x5140,0x5240,0x5340,0x5440,0x5540,0x5640,0x5740,
  0x5840,0x5940,0x5A40,0x5B40,0x5C40,0x5D40,0x5E40,0x5F40,
  0x6040,0x6140,0x6240,0x6340,0x6440,0x6540,0x6640,0x6740,
  0x6840,0x6940,0x6A40,0x6B40,0x6C40,0x6D40,0x6E40,0x6F40,
  0x7040,0x7140,0x7240,0x7340,0x7440,0x7540,0x7640,0x7740,
  0x7840,0x7940,0x7A40,0x7B40,0x7C40,0x7D40,0x7E40,0x7F40,
  0x8040,0x8140,0x8240,0x8340,0x8440,0x8540,0x8640,0x8740,
  0x8840,0x8940,0x8A40,0x8B40,0x8C40,0x8D40,0x8E40,0x8F40,
  0x9040,0x9140,0x9240,0x9340,0x9440,0x9540,0x9640,0x9740,
  0x9840,0x9940,0x9A40,0x9B40,0x9C40,0x9D40,0x9E40,0x9F40,
  0xA040,0xA140,0xA240,0xA340,0xA440,0xA540,0xA640,0xA740,
  0xA840,0xA940,0xAA40,0xAB40,0xAC40,0xAD40,0xAE40,0xAF40,
  0xB040,0xB140,0xB240,0xB340,0xB440,0xB540,0xB640,0xB740,
  0xB840,0xB940,0xBA40,0xBB40,0xBC40,0xBD40,0xBE40,0xBF40,
  0xC040,0xC140,0xC240,0xC340,0xC440,0xC540,0xC640,0xC740,
  0xC840,0xC940,0xCA40,0xCB40,0xCC40,0xCD40,0xCE40,0xCF40,
  0xD040,0xD140,0xD240,0xD340,0xD440,0xD540,0xD640,0xD740,
  0xD840,0xD940,0xDA40,0xDB40,0xDC40,0xDD40,0xDE40,0xDF40,
  0xE040,0xE140,0xE240,0xE340,0xE440,0xE540,0xE640,0xE740,
  0xE840,0xE940,0xEA40,0xEB40,0xEC40,0xED40,0xEE40,0xEF40,
  0xF040,0xF140,0xF240,0xF340,0xF440,0xF540,0xF640,0xF740,
  0xF840,0xF940,0xFA40,0xFB40,0xFC40,0xFD40,0xFE40,0xFF40,
  0xA050,0xA150,0xA250,0xA350,0xA450,0xA550,0xA650,0xA750,
  0xA850,0xA950,0xAA50,0xAB50,0xAC50,0xAD50,0xAE50,0xAF50,
  0xB050,0xB150,0xB250,0xB350,0xB450,0xB550,0xB650,0xB750,
  0xB850,0xB950,0xBA50,0xBB50,0xBC50,0xBD50,0xBE50,0xBF50,
  0xC050,0xC150,0xC250,0xC350,0xC450,0xC550,0xC650,0xC750,
  0xC850,0xC950,0xCA50,0xCB50,0xCC50,0xCD50,0xCE50,0xCF50,
  0xD050,0xD150,0xD250,0xD350,0xD450,0xD550,0xD650,0xD750,
  0xD850,0xD950,0xDA50,0xDB50,0xDC50,0xDD50,0xDE50,0xDF50,
  0xE050,0xE150,0xE250,0xE350,0xE450,0xE550,0xE650,0xE750,
  0xE850,0xE950,0xEA50,0xEB50,0xEC50,0xED50,0xEE50,0xEF50,
  0xF050,0xF150,0xF250,0xF350,0xF450,0xF550,0xF650,0xF750,
  0xF850,0xF950,0xFA50,0xFB50,0xFC50,0xFD50,0xFE50,0xFF50,
  0x00D0,0x0150,0x0250,0x0350,0x0450,0x0550,0x0650,0x0750,
  0x0850,0x0950,0x0A50,0x0B50,0x0C50,0x0D50,0x0E50,0x0F50,
  0x1050,0x1150,0x1250,0x1350,0x1450,0x1550,0x1650,0x1750,
  0x1850,0x1950,0x1A50,0x1B50,0x1C50,0x1D50,0x1E50,0x1F50,
  0x2050,0x2150,0x2250,0x2350,0x2450,0x2550,0x2650,0x2750,
  0x2850,0x2950,0x2A50,0x2B50,0x2C50,0x2D50,0x2E50,0x2F50,
  0x3050,0x3150,0x3250,0x3350,0x3450,0x3550,0x3650,0x3750,
  0x3850,0x3950,0x3A50,0x3B50,0x3C50,0x3D50,0x3E50,0x3F50,
  0x4050,0x4150,0x4250,0x4350,0x4450,0x4550,0x4650,0x4750,
  0x4850,0x4950,0x4A50,0x4B50,0x4C50,0x4D50,0x4E50,0x4F50,
  0x5050,0x5150,0x5250,0x5350,0x5450,0x5550,0x5650,0x5750,
  0x5850,0x5950,0x5A50,0x5B50,0x5C50,0x5D50,0x5E50,0x5F50,
  0x6050,0x6150,0x6250,0x6350,0x6450,0x6550,0x6650,0x6750,
  0x6850,0x6950,0x6A50,0x6B50,0x6C50,0x6D50,0x6E50,0x6F50,
  0x7050,0x7150,0x7250,0x7350,0x7450,0x7550,0x7650,0x7750,
  0x7850,0x7950,0x7A50,0x7B50,0x7C50,0x7D50,0x7E50,0x7F50,
  0x8050,0x8150,0x8250,0x8350,0x8450,0x8550,0x8650,0x8750,
  0x8850,0x8950,0x8A50,0x8B50,0x8C50,0x8D50,0x8E50,0x8F50,
  0x9050,0x9150,0x9250,0x9350,0x9450,0x9550,0x9650,0x9750,
  0x9850,0x9950,0x9A50,0x9B50,0x9C50,0x9D50,0x9E50,0x9F50,
  0xFA40,0xFB40,0xFC40,0xFD40,0xFE40,0xFF40,0x00C0,0x0140,
  0x0240,0x0340,0x0440,0x0540,0x0640,0x0740,0x0840,0x0940,
  0x0A40,0x0B40,0x0C40,0x0D40,0x0E40,0x0F40,0x1040,0x1140,
  0x1240,0x1340,0x1440,0x1540,0x1640,0x1740,0x1840,0x1940,
  0x1A40,0x1B40,0x1C40,0x1D40,0x1E40,0x1F40,0x2040,0x2140,
  0x2240,0x2340,0x2440,0x2540,0x2640,0x2740,0x2840,0x2940,
  0x2A40,0x2B40,0x2C40,0x2D40,0x2E40,0x2F40,0x3040,0x3140,
  0x3240,0x3340,0x3440,0x3540,0x3640,0x3740,0x3840,0x3940,
  0x3A40,0x3B40,0x3C40,0x3D40,0x3E40,0x3F40,0x4040,0x4140,
  0x4240,0x4340,0x4440,0x4540,0x4640,0x4740,0x4840,0x4940,
  0x4A40,0x4B40,0x4C40,0x4D40,0x4E40,0x4F40,0x5040,0x5140,
  0x5240,0x5340,0x5440,0x5540,0x5640,0x5740,0x5840,0x5940,
  0x5A40,0x5B40,0x5C40,0x5D40,0x5E40,0x5F40,0x6040,0x6140,
  0x6240,0x6340,0x6440,0x6540,0x6640,0x6740,0x6840,0x6940,
  0x6A40,0x6B40,0x6C40,0x6D40,0x6E40,0x6F40,0x7040,0x7140,
  0x7240,0x7340,0x7440,0x7540,0x7640,0x7740,0x7840,0x7940,
  0x7A40,0x7B40,0x7C40,0x7D40,0x7E40,0x7F40,0x8040,0x8140,
  0x8240,0x8340,0x8440,0x8540,0x8640,0x8740,0x8840,0x8940,
  0x8A40,0x8B40,0x8C40,0x8D40,0x8E40,0x8F40,0x9040,0x9140,
  0x9240,0x9340,0x9440,0x9540,0x9640,0x9740,0x9840,0x9940,
  0x9A40,0x9B40,0x9C40,0x9D40,0x9E40,0x9F40,0xA040,0xA140,
  0xA240,0xA340,0xA440,0xA540,0xA640,0xA740,0xA840,0xA940,
  0xAA40,0xAB40,0xAC40,0xAD40,0xAE40,0xAF40,0xB040,0xB140,
  0xB240,0xB340,0xB440,0xB540,0xB640,0xB740,0xB840,0xB940,
  0xBA40,0xBB40,0xBC40,0xBD40,0xBE40,0xBF40,0xC040,0xC140,
  0xC240,0xC340,0xC440,0xC540,0xC640,0xC740,0xC840,0xC940,
  0xCA40,0xCB40,0xCC40,0xCD40,0xCE40,0xCF40,0xD040,0xD140,
  0xD240,0xD340,0xD440,0xD540,0xD640,0xD740,0xD840,0xD940,
  0xDA40,0xDB40,0xDC40,0xDD40,0xDE40,0xDF40,0xE040,0xE140,
  0xE240,0xE340,0xE440,0xE540,0xE640,0xE740,0xE840,0xE940,
  0xEA40,0xEB40,0xEC40,0xED40,0xEE40,0xEF40,0xF040,0xF140,
  0xF240,0xF340,0xF440,0xF540,0xF640,0xF740,0xF840,0xF940,
  0x9A50,0x9B50,0x9C50,0x9D50,0x9E50,0x9F50,0xA050,0xA150,
  0xA250,0xA350,0xA450,0xA550,0xA650,0xA750,0xA850,0xA950,
  0xAA50,0xAB50,0xAC50,0xAD50,0xAE50,0xAF50,0xB050,0xB150,
  0xB250,0xB350,0xB450,0xB550,0xB650,0xB750,0xB850,0xB950,
  0xBA50,0xBB50,0xBC50,0xBD50,0xBE50,0xBF50,0xC050,0xC150,
  0xC250,0xC350,0xC450,0xC550,0xC650,0xC750,0xC850,0xC950,
  0xCA50,0xCB50,0xCC50,0xCD50,0xCE50,0xCF50,0xD050,0xD150,
  0xD250,0xD350,0xD450,0xD550,0xD650,0xD750,0xD850,0xD950,
  0xDA50,0xDB50,0xDC50,0xDD50,0xDE50,0xDF50,0xE050,0xE150,
  0xE250,0xE350,0xE450,0xE550,0xE650,0xE750,0xE850,0xE950,
  0xEA50,0xEB50,0xEC50,0xED50,0xEE50,0xEF50,0xF050,0xF150,
  0xF250,0xF350,0xF450,0xF550,0xF650,0xF750,0xF850,0xF950,
  0xFA50,0xFB50,0xFC50,0xFD50,0xFE50,0xFF50,0x00D0,0x0150,
  0x0250,0x0350,0x0450,0x0550,0x0650,0x0750,0x0850,0x0950,
  0x0A50,0x0B50,0x0C50,0x0D50,0x0E50,0x0F50,0x1050,0x1150,
  0x1250,0x1350,0x1450,0x1550,0x1650,0x1750,0x1850,0x1950,
  0x1A50,0x1B50,0x1C50,0x1D50,0x1E50,0x1F50,0x2050,0x2150,
  0x2250,0x2350,0x2450,0x2550,0x2650,0x2750,0x2850,0x2950,
  0x2A50,0x2B50,0x2C50,0x2D50,0x2E50,0x2F50,0x3050,0x3150,
  0x3250,0x3350,0x3450,0x3550,0x3650,0x3750,0x3850,0x3950,
  0x3A50,0x3B50,0x3C50,0x3D50,0x3E50,0x3F50,0x4050,0x4150,
  0x4250,0x4350,0x4450,0x4550,0x4650,0x4750,0x4850,0x4950,
  0x4A50,0x4B50,0x4C50,0x4D50,0x4E50,0x4F50,0x5050,0x5150,
  0x5250,0x5350,0x5450,0x5550,0x5650,0x5750,0x5850,0x5950,
  0x5A50,0x5B50,0x5C50,0x5D50,0x5E50,0x5F50,0x6050,0x6150,
  0x6250,0x6350,0x6450,0x6550,0x6650,0x6750,0x6850,0x6950,
  0x6A50,0x6B50,0x6C50,0x6D50,0x6E50,0x6F50,0x7050,0x7150,
  0x7250,0x7350,0x7450,0x7550,0x7650,0x7750,0x7850,0x7950,
  0x7A50,0x7B50,0x7C50,0x7D50,0x7E50,0x7F50,0x8050,0x8150,
  0x8250,0x8350,0x8450,0x8550,0x8650,0x8750,0x8850,0x8950,
  0x8A50,0x8B50,0x8C50,0x8D50,0x8E50,0x8F50,0x9050,0x9150,
  0x9250,0x9350,0x9450,0x9550,0x9650,0x9750,0x9850,0x9950
};
#endif
