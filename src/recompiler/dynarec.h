#ifndef DYNAREC_H
#define DYNAREC_H
#include "cache.h"
#include "../generic_cpu.h"
#include "../states.h"
#include "../mem.h"
#include "../keypad.h"
#include "../lcd.h"
#include "../sound.h"
#include "../throttle.h"

const static array<uint8, 16> conditionals {0x20, 0x28, 0x30, 0x38, 0xc0, 0xc2, 0xc4, 0xc8, 0xca, 0xcc, 0xd0, 0xd2, 0xd4, 0xd8, 0xda, 0xdc};
const static array<uint8, 14> jumps {0x18, 0xc3, 0xc7, 0xc9, 0xcd, 0xcf, 0xd7, 0xd9, 0xdf, 0xe7, 0xe9, 0xef, 0xf7, 0xff};

class dynarec_cpu : public generic_cpu {
  public:
    dynarec_cpu();
    ~dynarec_cpu();
    void emulate(mem &m, keypad &k, lcd &l, sound &s);
    optional<cache_block*> translate(uint16 address, mem &m, keypad &k, lcd &l);
    void emit_code(void);
  private:
    jit_type_t type_uint8_ptr, type_uint16_ptr, type_class_ptr;
    jit_type_t read_byte_signature, read_word_signature, write_byte_signature, write_word_signature, update_timers_signature, step_lcd_signature, handle_events_signature, throttle_signature;
};

//these macros start with JIT_ to avoid having to redefine the macros in cpu.h
//however these should only be used *outside* JIT code
//I still need to figure out how to handle updating the aux classes based on the dt
//from all jumps and conditionals, the variable dt in the conditionals below is commented out for now
#define JIT_JR(n) do {pc.w += ((signed char)n);} while(0)

#define JIT_COND_JR(cond,n) do {if (cond) {JIT_JR(n);/*dt += 1;*/};} while(0)

#define JIT_RET do {JIT_POP;} while(0)

#define JIT_COND_RET(cond) do {if (cond) {JIT_RET;/*dt += 3;*/};} while(0)

#define JIT_RETI do {ime=1;JIT_POP;} while(0)

#define JIT_JP(n) do {pc.w = n;} while(0)

#define JIT_COND_JP(cond,n) do {if (cond) {JIT_JP(n);/*dt += 1;*/};} while(0)

#define JIT_CALL(n) do {JIT_PUSH; pc.w = n;} while(0)

#define JIT_COND_CALL(cond,n) do {if (cond) {JIT_CALL(n);/*dt += 3;*/};} while(0)

#define JIT_POP do {pc.w = m.read_word(sp.w); sp.w += 2;} while(0)

#define JIT_PUSH do {sp.w -= 2; m.write_word(sp.w, pc.w + 3);} while (0)

#define JIT_RST(n) do {JIT_PUSH; pc.w = n;} while(0)

#endif
