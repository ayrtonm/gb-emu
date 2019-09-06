#ifndef DYNAREC_H
#define DYNAREC_H
#include "cache.h"
#include "../mem.h"
#include "../keypad.h"
#include "../lcd.h"
#include "../sound.h"

const static array<uint8, 16> cond_branches {0x20, 0x28, 0x30, 0x38, 0xc0, 0xc2, 0xc4, 0xc8, 0xca, 0xcc, 0xd0, 0xd2, 0xd4, 0xd8, 0xda, 0xdc};
const static array<uint8, 14> jumps {0x18, 0xc3, 0xc7, 0xc9, 0xcd, 0xcf, 0xd7, 0xd9, 0xdf, 0xe7, 0xe9, 0xef, 0xf7, 0xff};

class dynarec_cpu {
  public:
    dynarec_cpu();
    ~dynarec_cpu();
    void emulate(mem &m, keypad &k, lcd &l, sound &s);
    cache_block translate(uint16 start_address, mem &m, jit_context *context);
    void emit_code(void);
  private:
    word16 af, bc, de, hl, sp;
    uint8 ime, ei_delay, halt;
    jit_type_t type_uint8_ptr, type_uint16_ptr, type_class_ptr;
    jit_type_t read_byte_signature, read_word_signature, write_byte_signature, write_word_signature;
};

#endif
