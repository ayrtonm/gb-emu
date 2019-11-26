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

const static array<uint8_t, 16> conditionals {0x20, 0x28, 0x30, 0x38, 0xc0, 0xc2, 0xc4, 0xc8, 0xca, 0xcc, 0xd0, 0xd2, 0xd4, 0xd8, 0xda, 0xdc};
const static array<uint8_t, 14> jumps {0x18, 0xc3, 0xc7, 0xc9, 0xcd, 0xcf, 0xd7, 0xd9, 0xdf, 0xe7, 0xe9, 0xef, 0xf7, 0xff};

class dynarec_cpu : public generic_cpu {
  public:
    dynarec_cpu();
    ~dynarec_cpu();
    long int emulate(mem &m, keypad &k, lcd &l, sound &s, long int limit);
    optional<cache_block*> translate(uint16_t address, mem &m, keypad &k, lcd &l);
    void emit_code(void);
  private:
    jit_type_t type_uint8_t_ptr, type_uint16_t_ptr, type_class_ptr;
    jit_type_t read_byte_signature, read_word_signature,
               write_byte_signature, write_word_signature,
               invalidate_blocks_signature,
               update_timers_signature,
               step_lcd_signature,
               handle_events_signature,
               throttle_signature;
};

#endif
