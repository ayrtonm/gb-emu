#ifndef Z80_H
#define Z80_H
#include "mem.h"

//this file contains opcode macros 
//separated into normal and 0xCB sections

//BIT(n) is deprecated
//use hexdecimal instead
#define BIT(n)		(1 << (n))
//ability to mask multiple bits is useful for flags
#define GET(a,b)	((a) & (b))
#define SET(b,a)	((a) |= (b))
#define CLEAR(b,a)	((a) &= ~(b))
#define TOGGLE(b,a)	((a) ^= (b))
//CHBIT(n,x,y) changes bit(s) n of x to 0 if y == 0
//or to 1 if y == ANYTHING ELSE
#define CHBIT(n,x,y)	((y) == 0 ? CLEAR((n),(x)) : SET((n),(x)))
#define SWAP(x)		((x)>>4|(x)<<4)/*8 bit only*/

//size in bits
#define SIZE(x)		((sizeof(x))<<3)
//lastbit number
#define LASTBIT(x)	((SIZE(x))-1)
//"returns" lastbit value
#define CHECKNEG(x)	(GET(LASTBIT(x),x) == 0 ? 0 : 1)

/*16 bit only*/
#define LOW(x)		((x) & 0xFF)
#define HIGH(x)		((x) >> 8)

/*flags*/
#define Z_FLAG	0x80
#define N_FLAG	0x40
#define H_FLAG	0x20
#define C_FLAG	0x10
#define CARRY	((GET(C_FLAG,_F))>>4)
#define ZERO	((GET(Z_FLAG,_F))>>7)

/*registers*/
#define _A	(Z->AF.B.h)
#define _F	(Z->AF.B.l)
#define _B	(Z->BC.B.h)
#define _C	(Z->BC.B.l)
#define _D	(Z->DE.B.h)
#define _E	(Z->DE.B.l)
#define _H	(Z->HL.B.h)
#define _L	(Z->HL.B.l)
#define _AF	(Z->AF.W)
#define _BC	(Z->BC.W)
#define _DE	(Z->DE.W)
#define _HL	(Z->HL.W)
#define _PC	(Z->PC.W)
#define _SP	(Z->SP.W)
#define _IME	(Z->IME)

#define IMM8 (read_byte(_PC-1))
#define IMM16 (read_word(_PC-2))

typedef union
{
  uint16 W;
  struct
  {
    #ifdef HIGH_ENDIAN
    uint8 h,l;
    #else
    uint8 l,h;
    #endif
  } B;
} word16;
typedef struct z80
{
  word16 AF,BC,DE,HL,PC,SP;
  uint8 IME;
} z80;

static uint8 length[0x0100] = 
{
/*0 1 2 3 4 5 6 7 8 9 A B C D E F*/
  1,3,1,1,1,1,2,1,3,1,1,1,1,1,2,1, /*0x*/
  2,3,1,1,1,1,2,1,2,1,1,1,1,1,2,1, /*1x*/
  2,3,1,1,1,1,2,1,2,1,1,1,1,1,2,1, /*2x*/
  2,3,1,1,1,1,2,1,2,1,1,1,1,1,2,1, /*3x*/
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, /*4x*/
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, /*5x*/
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, /*6x*/
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, /*7x*/
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, /*8x*/
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, /*9x*/
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, /*Ax*/
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, /*Bx*/
  1,1,3,3,3,1,2,1,1,1,3,1,3,3,2,1, /*Cx*/
  1,1,3,0,3,1,2,1,1,1,3,0,3,0,2,1, /*Dx*/
  2,1,2,0,0,1,2,1,2,1,3,0,0,0,2,1, /*Ex*/
  2,1,2,1,0,1,2,1,2,1,3,1,0,0,2,1  /*Fx*/
};

static uint8 cb_cycles[0x08] = {2,2,2,2,2,2,4,2};

static uint8 cycles[0x0100] =
{
/*0 1 2 3 4 5 6 7 8 9 A B C D E F*/
  1,3,2,2,1,1,2,1,5,2,2,2,1,1,2,1, /*0x*/
  1,3,2,2,1,1,2,1,3,2,2,2,1,1,2,1, /*1x*/
  1,3,2,2,1,1,2,1,3,2,2,2,1,1,2,1, /*2x*/
  1,3,2,2,1,1,2,1,3,2,2,2,1,1,2,1, /*3x*/
  1,1,1,1,1,1,2,1,1,1,1,1,1,1,2,1, /*4x*/
  1,1,1,1,1,1,2,1,1,1,1,1,1,1,2,1, /*5x*/
  1,1,1,1,1,1,2,1,1,1,1,1,1,1,2,1, /*6x*/
  2,2,2,2,2,2,1,2,1,1,1,1,1,1,2,1, /*7x*/
  1,1,1,1,1,1,2,1,1,1,1,1,1,1,2,1, /*8x*/
  1,1,1,1,1,1,2,1,1,1,1,1,1,1,2,1, /*9x*/
  1,1,1,1,1,1,2,1,1,1,1,1,1,1,2,1, /*Ax*/
  1,1,1,1,1,1,2,1,1,1,1,1,1,1,2,1, /*Bx*/
  2,3,3,4,3,4,2,4,2,4,3,1,3,6,2,4, /*Cx*/
  2,3,3,0,3,4,2,4,2,4,3,0,3,0,2,4, /*Dx*/
  3,3,2,0,0,4,2,4,4,1,4,0,0,0,2,4, /*Ex*/
  3,3,2,1,0,4,2,4,3,2,4,1,0,0,2,4  /*Fx*/
};

//regular opcodes
//todo first understand, then write daa (bcd) macro

#define ADD(r) \
  uint16 mtemp=_A+(r);\
  CLEAR(N_FLAG,_F);\
  if(mtemp==0) {SET(Z_FLAG,_F);} else {CLEAR(Z_FLAG,_F);}\
  if(mtemp>0xFF) {SET(C_FLAG,_F);} else {CLEAR(C_FLAG,_F);}\
  if((_A & 0x0F) > (mtemp & 0x0F)) {SET(H_FLAG,_F);} else {CLEAR(H_FLAG,_F);}\
  _A = (mtemp & 0xFF)

#define ADC(r) \
  uint16 mtemp=_A+(r)+CARRY;\
  CLEAR(N_FLAG,_F);\
  if(mtemp==0) {SET(Z_FLAG,_F);} else {CLEAR(Z_FLAG,_F);}\
  if(mtemp>0xFF) {SET(C_FLAG,_F);} else {CLEAR(C_FLAG,_F);}\
  if((_A & 0x0F) > (mtemp & 0x0F)) {SET(H_FLAG,_F);} else {CLEAR(H_FLAG,_F);}\
  _A = (mtemp & 0xFF)

#define SUB(r) \
  uint16 mtemp=_A-(r);\
  SET(N_FLAG,_F);\
  if(mtemp==0) {SET(Z_FLAG,_F);} else {CLEAR(Z_FLAG,_F);}\
  if(CHECKNEG(mtemp)) {SET(C_FLAG,_F);} else {CLEAR(C_FLAG,_F);}\
  if((_A & 0x0F) > (mtemp & 0x0F)) {SET(H_FLAG,_F);} else {CLEAR(H_FLAG,_F);}\
  _A = (mtemp & 0xFF)

#define SBC(r) \
  uint16 mtemp=_A-((r)+CARRY);\
  SET(N_FLAG,_F);\
  if(mtemp==0) {SET(Z_FLAG,_F);} else {CLEAR(Z_FLAG,_F);}\
  if(CHECKNEG(mtemp)) {SET(C_FLAG,_F);} else {CLEAR(C_FLAG,_F);}\
  if((_A & 0x0F) > (mtemp & 0x0F)) {SET(H_FLAG,_F);} else {CLEAR(H_FLAG,_F);}\
  _A = (mtemp & 0xFF)

#define AND(r) \
  _A &= (r);\
  if (_A==0) {SET(Z_FLAG,_F);} else {CLEAR(Z_FLAG,_F);}\
  CLEAR(N_FLAG|C_FLAG,_F);\
  SET(H_FLAG,_F)

#define EOR(r) \
  _A ^= (r);\
  if (_A==0) {SET(Z_FLAG,_F);} else {CLEAR(Z_FLAG,_F);}\
  CLEAR(N_FLAG|H_FLAG|C_FLAG,_F)

#define OR(r) \
  _A |= (r);\
  if (_A==0) {SET(Z_FLAG,_F);} else {CLEAR(Z_FLAG,_F);}\
  CLEAR(N_FLAG|H_FLAG|C_FLAG,_F)

#define CP(r) \
  uint16 mtemp = _A-(r);\
  SET(N_FLAG,_F);\
  if(mtemp==0) {SET(Z_FLAG,_F);} else {CLEAR(Z_FLAG,_F);}\
  if(CHECKNEG(mtemp)) {SET(C_FLAG,_F);} else {CLEAR(C_FLAG,_F);}\
  if((_A & 0x0F) > (mtemp & 0x0F)) {SET(H_FLAG,_F);} else {CLEAR(H_FLAG,_F);}

#define INC(r) \
  uint8 mtemp = (r)+1;\
  if ((r) == 0) {SET(Z_FLAG,_F);} else {CLEAR(Z_FLAG,_F);}\
  CLEAR(N_FLAG,_F);\
  if (((r) & 0x0F) > (mtemp & 0x0F)) {SET(H_FLAG,_F);} else {CLEAR(H_FLAG,_F);}\
  r = mtemp

#define DEC(r) \
  uint8 mtemp = (r)-1;\
  if ((r) == 0) {SET(Z_FLAG,_F);} else {CLEAR(Z_FLAG,_F);}\
  CLEAR(N_FLAG,_F);\
  if (((r) & 0x0F) > (mtemp & 0x0F)) {SET(H_FLAG,_F);} else {CLEAR(H_FLAG,_F);}\
  r = mtemp

#define ADDHL(r) \
  int mtemp = _HL+(r);\
  CLEAR(N_FLAG,_F);\
  if (mtemp > 0xFFFF) {SET(C_FLAG,_F);}\
  if ((_HL & 0x0F) > (mtemp & 0x0F)) {SET(H_FLAG,_F);}\
  _HL = (mtemp & 0xFFFF)

//LD(x,y) is for ALL n-bit register to n-bit register loads
//it is also used for IMMn to n-bit register loads
#define LD(x,y) \
  x=y
//the next two macros are similar and may seem backwards
//since the 1st arg is the 2nd letter in the macro name
//but they are correct, just think about it
//LD_MR(x,y) is for memory to register loads (x6 and xE between 4x and 7x)
#define LD_MR(x,y) \
  x=read_byte(y)
//LD_RM is for register to memory loads (7x)
#define LD_RM(x,y) \
  write_byte(x,y)

#define PUSH(r) \
  _SP-=2;\
  write_word(_SP,r)
//todo fix POP(_AF) add flags?
//does this actually need to be fixed
//or is _F only changing because of POP(_AF)?
#define POP(r) \
  r = read_word(_SP);\
  _SP+=2
//todo double check time increments for opcodes
//especially conditional increments (JP,JR,RET,CALL,...)
#define CALL(n) \
  _SP-=2;\
  write_word(_SP,_PC);\
  _PC=n
#define COND_CALL(cond,n) \
  if (cond) {CALL(n);dt+=3;}
#define JP(n) \
  _PC=n
#define COND_JP(cond,n) \
  if (cond) {JP(n);dt+=1;}

/*JR(n)
 check bit 7
 if 0 jp value
 if 1 pc -= new value*/
#define JR(n) \
  uint8 mtemp = n;\
  if (CHECKNEG(mtemp)==0) {JP(mtemp);}\
  else {mtemp = ~mtemp+1;_PC-=mtemp;}
#define COND_JR(cond,n) \
  if (cond) {JR(n);dt+=1;}
#define RST(n) \
  CALL(n)
#define RET \
  _PC=read_word(_SP);\
  _SP+=2
#define COND_RET(cond) \
  if (cond) {RET;dt+=3;}
#define RETI \
  RET;EI_OP
#define EI_OP \
  _IME = 1
#define DI_OP \
  _IME = 0

//0xCB opcodes
//use CHBIT() instead of if statements
#define RLCA RLC(_A);CLEAR(Z_FLAG,_F)
#define RRCA RRC(_A);CLEAR(Z_FLAG,_F)
#define RLA RL(_A);CLEAR(Z_FLAG,_F)
#define RRA RR(_A);CLEAR(Z_FLAG,_F)
#define RLC(r) \
  CHBIT(C_FLAG,_F,GET(0x80,r));\
  (r) = (r) << 1;\
  CHBIT(0,r,CARRY);\
  CHBIT(Z_FLAG,_F,r);\
  CLEAR((N_FLAG|H_FLAG),_F)
#define RRC(r) \
  CHBIT(C_FLAG,_F,GET(0x01,r));\
  (r) = (r) >> 1;\
  CHBIT(0x80,r,CARRY);\
  CHBIT(Z_FLAG,_F,r);\
  CLEAR((N_FLAG|H_FLAG),_F)
#define RL(r) \
  uint8 ctemp = CARRY;\
  CHBIT(C_FLAG,_F,GET(0x80,r));\
  (r) = (r) << 1;\
  CHBIT(0x01,r,ctemp);\
  CHBIT(Z_FLAG,_F,r)
#define RR(r) \
  uint8 ctemp = CARRY;\
  CHBIT(C_FLAG,_F,GET(0x01,r));\
  (r) = (r) >> 1;\
  CHBIT(0x80,r,ctemp);\
  CHBIT(Z_FLAG,_F,r)
#define CB_SWAP(r) \
  r = SWAP(r);\
  CHBIT(Z_FLAG,_F,r);\
  CLEAR((N_FLAG|H_FLAG|C_FLAG),_F)
#define SLA(r) \
  CHBIT(C_FLAG,_F,GET(0x80,r));\
  (r) = (r) << 1;\
  CLEAR(0x01,r);\
  CHBIT(Z_FLAG,_F,r);\
  CLEAR((N_FLAG|H_FLAG),_F)
#define SRL(r) \
  CHBIT(C_FLAG,_F,GET(0x01,r));\
  (r) = (r) >> 1;\
  CLEAR(0x80,r);\
  CHBIT(Z_FLAG,_F,r);\
  CLEAR((N_FLAG|H_FLAG),_F)
#define SRA(r) \
  CHBIT(C_FLAG,_F,GET(0x01,r));\
  (r) = (r) >> 1;\
  CHBIT(0x80,r,GET(0x40,r));\
  CHBIT(Z_FLAG,_F,r);\
  CLEAR((N_FLAG|H_FLAG),_F)

extern z80 *init_z80(void);
extern int emulate(z80 *);

#endif
