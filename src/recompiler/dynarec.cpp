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
  //pc.w = 0x0100;
  init_address = 0x0100;
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
}

void dynarec_cpu::emulate(mem &m, keypad &k, lcd &l, sound &s) {
  //initialize libjit stuff
  jit_context context;

  cache *storage = new cache();
  optional<int> idx;
  uint16 address = init_address;
  cache_block block(context);

  for (;;) {
    idx = storage->find_block(address);
    if (!idx) {
      block = translate(address, m, k, l, &context);
      while (!block.is_valid()) {
        switch (m.read_byte(address)) {
          #include "jumps.h"
          #include "cond_branches.h"
          default: {
            cout << "ran into unimplemented jump or conditional branch" << endl;
            break;
          }
        }
        idx = storage->find_block(address);
        if (!idx) {
          block = translate(address, m, k, l, &context);
          idx = storage->insert_block(block);
        }
        else {
          break;
        }
      }
    }
    address = storage->exec_block(idx.value());
  }
  delete storage;
}

cache_block dynarec_cpu::translate(uint16 address, mem &m, keypad &k, lcd &l, jit_context *context) {
  cache_block block(*context);
  context->build_start();
  block.set_start(address);
  jit_value m_addr = block.new_constant(&m, type_class_ptr);
  jit_value k_addr = block.new_constant(&k, type_class_ptr);
  jit_value l_addr = block.new_constant(&l, type_class_ptr);
  jit_value tp_addr = block.new_constant(&tp, type_class_ptr);
  uint8 opcode = m.read_byte(address);
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
  if (block.get_end() != block.get_start()) {
    block.compile();
    block.bind();
  }
  else {
    block.invalidate();
  }
  context->build_end();
  return block;
}
