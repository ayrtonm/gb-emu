#ifdef DYNAREC_CPU
#define DYNAREC_DEBUG
#include <iostream>
#include <algorithm>
#include <jit/jit-plus.h>
#include "dynarec.h"
#include "jit_macros.h"

using namespace std;

bool is_cond(uint8 opcode) {
  return find(begin(conditionals), end(conditionals), opcode) != end(conditionals);
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
  optional<cache_block*> block;

  for (;;) {
    //if we can't find the right block in the cache
    for (idx = storage->find_block(pc.w); !idx; idx = storage->find_block(pc.w)) {
      //try translating the binary
      block = translate(pc.w, m, k, l);
      //if $pc points to a jump/conditional
      if (!block) {
        //update $pc accordingly
        uint8 op = m.read_byte(pc.w);
        switch(op) {
          #include "jumps.h"
          #include "conditionals.h"
          default: {
            cout << "ran into unimplemented jump or conditional branch" << endl;
            break;
          }
        }
        //at this point $pc may or may not point to another jump/conditional so we search the cache again
      }
      //if the translation was successful
      else {
        //cache the translated block
        idx = storage->insert_block(block.value());
        //break out of the for loop to execute the block
        break;
      }
    }
    //at this point idx points to the correct block in the cache
    //execute the block and update pc.w accordingly
    pc.w = storage->exec_block(idx.value());
  }
  delete storage;
  return;
}

/*
LibJIT's function destructors do not actually delete the underlying raw function until the context is destroyed
since we want to be able to replace blocks individually when the cache gets filled, this means that we need a JIT
context for each block. As a result, there is quite a bit of repetition for now
*/
optional<cache_block*> dynarec_cpu::translate(uint16 address, mem &m, keypad &k, lcd &l) {
  //check if the translation is going to fail
  uint8 opcode = m.read_byte(address);
  if (is_cond(opcode) || is_jump(opcode)) {
    return nullopt;
  }
  //we are sure the translation won't fail at this point
  //let's create a new context for this block
  jit_context *cx = new jit_context();
  //let's allocate memory for the new block here
  cache_block *block = new cache_block(*cx);
  block->set_start(address);
  block->build_start();

  //get the addresses of the four auxilary classes
  //we have to do this for each block since they don't share a context
  jit_value m_addr = block->new_constant(&m, type_class_ptr);
  jit_value k_addr = block->new_constant(&k, type_class_ptr);
  jit_value l_addr = block->new_constant(&l, type_class_ptr);
  jit_value tp_addr = block->new_constant(&tp, type_class_ptr);

  //read in opcodes until we hit a conditional or jump
  //we are guaranteed to have at least one valid instruction due to the check at the beginning of this function
  while (!(is_cond(opcode)||is_jump(opcode))) {
#ifdef DYNAREC_DEBUG
    //storing the opcode isn't strictly necessary
    block->store_data(opcode);
#endif
    //store opcode arguments if any exist
    for (int i = 1; i < length[opcode]; i++) {
      address += 1;
      block->store_data(m.read_byte(address));
    }
    //translate an instruction into part of the block's JIT function
    switch(opcode) {
      #include "translations.h"
      default: {
        cout << "ran into unimplemented opcode:" << hex << (int)opcode << " at [" << hex << (int)address << "]" << endl;
        getchar();
        break;
      }
    }
    //insert calls to the auxilary classes' update functions in the block's JIT function
    //first get the arguments we are going to pass them
    //since these all depend on dt, we have to do this for each instruction
    jit_value dt = block->new_constant(cycles[opcode], jit_type_int);
    jit_value_t update_timers_args[] = {m_addr.raw(), dt.raw()};
    jit_value_t step_lcd_args[] = {l_addr.raw(), dt.raw(), m_addr.raw()};
    jit_value_t handle_events_args[] = {k_addr.raw(), dt.raw(), m_addr.raw()};
    jit_value_t throttle_args[] = {tp_addr.raw(), dt.raw()};

    //insert calls to the aux class update functions
    block->insn_call_native(NULL, (void *)&mem::update_timers, update_timers_signature, update_timers_args, 2, 0);
    block->insn_call_native(NULL, (void *)&lcd::step_lcd, step_lcd_signature, step_lcd_args, 3, 0);
    jit_value event = block->insn_call_native(NULL, (void *)&keypad::handle_events, handle_events_signature, handle_events_args, 3, 0);
    //FIXME: write a function to handle the result of keypad::handle_events() from the JIT code
    block->insn_call_native(NULL, (void *)&throttle_controller::throttle, throttle_signature, throttle_args, 2, 0);

    address++;
    opcode = m.read_byte(address);
  };
  //end_address is guaranteed to be a jump or conditional
  block->set_end(address);
  block->insn_return();
  block->compile();
  block->bind();
  block->build_end();
  return block;
}
#endif
