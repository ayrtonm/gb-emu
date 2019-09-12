#include <iostream>
#include <algorithm>
#include <jit/jit-plus.h>
#include "dynarec.h"

using namespace std;

bool is_cond(uint8 opcode) {
  return find(begin(cond_branches), end(cond_branches), opcode) != end(cond_branches);
}

bool is_jump(uint8 opcode) {
  return find(begin(jumps), end(jumps), opcode) != end(jumps);
}

dynarec_cpu::dynarec_cpu() {
  af.w = 0x01b0;
  bc.w = 0x0013;
  de.w = 0x00d8;
  hl.w = 0x014d;
  sp.w = 0xfffe;
  pc.w = 0x0100;
  halt = 0;
  ime = 0;
  ei_delay = 0;

  type_uint8_ptr = jit_type_create_pointer(jit_type_ubyte, 0);
  type_uint16_ptr = jit_type_create_pointer(jit_type_ushort, 0);
  type_class_ptr = jit_type_create_pointer(jit_type_void_ptr, 0);

  jit_type_t read_mem_args_t[] = {jit_type_void_ptr, jit_type_ushort};
  jit_type_t write_byte_args_t[] = {jit_type_void_ptr, jit_type_ushort, jit_type_ubyte};
  jit_type_t write_word_args_t[] = {jit_type_void_ptr, jit_type_ushort, jit_type_ushort};
  jit_type_t update_timers_args_t[] = {jit_type_void_ptr, jit_type_int};
  jit_type_t step_lcd_args_t[] = {jit_type_void_ptr, jit_type_int, jit_type_void_ptr};
  jit_type_t handle_events_args_t[] = {jit_type_void_ptr, jit_type_int, jit_type_void_ptr};
  jit_type_t throttle_args_t[] = {jit_type_void_ptr, jit_type_int};

  read_byte_signature = jit_type_create_signature(jit_abi_cdecl, jit_type_ubyte, read_mem_args_t, 2, 0);
  read_word_signature = jit_type_create_signature(jit_abi_cdecl, jit_type_ushort, read_mem_args_t, 2, 0);
  write_byte_signature = jit_type_create_signature(jit_abi_cdecl, jit_type_void, write_byte_args_t, 3, 0);
  write_word_signature = jit_type_create_signature(jit_abi_cdecl, jit_type_void, write_word_args_t, 3, 0);
  update_timers_signature = jit_type_create_signature(jit_abi_cdecl, jit_type_void, update_timers_args_t, 2, 0);
  step_lcd_signature = jit_type_create_signature(jit_abi_cdecl, jit_type_void, step_lcd_args_t, 3, 0);
  handle_events_signature = jit_type_create_signature(jit_abi_cdecl, jit_type_int, handle_events_args_t, 3, 0);
  throttle_signature = jit_type_create_signature(jit_abi_cdecl, jit_type_void, throttle_args_t, 2, 0);
  tp.start_timer();
}

dynarec_cpu::~dynarec_cpu() {
  jit_type_free(type_uint8_ptr);
  jit_type_free(type_uint16_ptr);
  jit_type_free(type_class_ptr);
  jit_type_free(read_byte_signature);
  jit_type_free(read_word_signature);
  jit_type_free(write_byte_signature);
  jit_type_free(write_word_signature);
  jit_type_free(update_timers_signature);
  jit_type_free(step_lcd_signature);
  jit_type_free(handle_events_signature);
  jit_type_free(throttle_signature);
}

void dynarec_cpu::emulate(mem &m, keypad &k, lcd &l, sound &s) {
  cache *storage = new cache();
  optional<int> idx;
  optional<cache_block> block;

  for (;;) {
    idx = storage->find_block(pc.w);
    if (!idx) {
      block = translate(pc.w, m, k, l);
      while (!block) {
        switch (m.read_byte(pc.w)) {
          #include "jumps.h"
          #include "cond_branches.h"
          default: {
            cout << "ran into unimplemented jump or conditional branch" << endl;
            break;
          }
        }
        idx = storage->find_block(pc.w);
        if (!idx) {
          block = translate(pc.w, m, k, l);
          idx = storage->insert_block(block.value());
        }
        else {
          break;
        }
      }
    }
    pc.w = storage->exec_block(idx.value());
  }
  delete storage;
  return;
}

optional<cache_block> dynarec_cpu::translate(uint16 address, mem &m, keypad &k, lcd &l) {
  //jit_function destructors do not free the raw function. the raw function persists until the context is destroyed
  //since we want to reuse the same context throughout the program, this means that we should not make instances of cache_blocks (class derived from jit_function) that we will not use
  //this means it is important to return nullopt in the case of a jump or conditional branch before the cache_block instance is created
  //this also means there is another potential memory leak issue when replacing one cache_block in the cache with a new one
  uint8 opcode = m.read_byte(address);
  if (is_cond(opcode) || is_jump(opcode)) {
    return nullopt;
  }
  cache_block block;
  block.set_start(address);
  block.build_start();
  jit_value m_addr = block.new_constant(&m, type_class_ptr);
  jit_value k_addr = block.new_constant(&k, type_class_ptr);
  jit_value l_addr = block.new_constant(&l, type_class_ptr);
  jit_value tp_addr = block.new_constant(&tp, type_class_ptr);
  while (!(is_cond(opcode)||is_jump(opcode))) {
    //the following line should only be used for debugging as storing opcodes is not necessary, storing arguments however will be useful
    block.store_data(opcode);
    //store opcode arguments if any exist
    for (int i = 1; i < length[opcode]; i++) {
      address += 1;
      block.store_data(m.read_byte(address));
    }
    switch(opcode) {
      #include "translations.h"
      default: {
        cout << "ran into unimplemented opcode" << endl;
        break;
      }
    }
    jit_value dt = block.new_constant(cycles[opcode], jit_type_int);
    jit_value_t update_timers_args[] = {m_addr.raw(), dt.raw()};
    jit_value_t step_lcd_args[] = {l_addr.raw(), dt.raw(), m_addr.raw()};
    jit_value_t handle_events_args[] = {k_addr.raw(), dt.raw(), m_addr.raw()};
    jit_value_t throttle_args[] = {tp_addr.raw(), dt.raw()};

    block.insn_call_native(NULL, (void *)&mem::update_timers, update_timers_signature, update_timers_args, 2, 0);
    block.insn_call_native(NULL, (void *)&lcd::step_lcd, step_lcd_signature, step_lcd_args, 3, 0);
    jit_value event = block.insn_call_native(NULL, (void *)&keypad::handle_events, handle_events_signature, handle_events_args, 3, 0);
    block.insn_call_native(NULL, (void *)&throttle_controller::throttle, throttle_signature, throttle_args, 2, 0);

    address++;
    opcode = m.read_byte(address);
  };
  block.set_end(address);
  block.insn_return();
  block.compile();
  block.bind();
  block.build_end();
  return block;
}
