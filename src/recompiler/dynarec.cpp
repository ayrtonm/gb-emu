#ifdef DYNAREC_CPU
#include <iostream>
#include <algorithm>
#include <jit/jit-plus.h>
#include "dynarec.h"
#include "jit_macros.h"

using namespace std;

bool is_cond(uint8_t opcode) {
  return find(begin(conditionals), end(conditionals), opcode) != end(conditionals);
}

bool is_jump(uint8_t opcode) {
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

  type_uint8_t_ptr = jit_type_create_pointer(jit_type_ubyte, 0);
  type_uint16_t_ptr = jit_type_create_pointer(jit_type_ushort, 0);
  type_class_ptr = jit_type_create_pointer(jit_type_void_ptr, 0);

  jit_type_t invalidate_blocks_args_t[] = {jit_type_ushort};
  jit_type_t read_mem_args_t[] = {jit_type_void_ptr, jit_type_ushort};
  jit_type_t write_byte_args_t[] = {jit_type_void_ptr, jit_type_ushort, jit_type_ubyte};
  jit_type_t write_word_args_t[] = {jit_type_void_ptr, jit_type_ushort, jit_type_ushort};
  jit_type_t update_timers_args_t[] = {jit_type_void_ptr, jit_type_int};
  jit_type_t step_lcd_args_t[] = {jit_type_void_ptr, jit_type_int, jit_type_void_ptr};
  jit_type_t handle_events_args_t[] = {jit_type_void_ptr, jit_type_int, jit_type_void_ptr};
  jit_type_t throttle_args_t[] = {jit_type_void_ptr, jit_type_int};

  invalidate_blocks_signature = jit_type_create_signature(jit_abi_cdecl, jit_type_void, invalidate_blocks_args_t, 1, 0);
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
  jit_type_free(type_uint8_t_ptr);
  jit_type_free(type_uint16_t_ptr);
  jit_type_free(type_class_ptr);
  jit_type_free(invalidate_blocks_signature);
  jit_type_free(read_byte_signature);
  jit_type_free(read_word_signature);
  jit_type_free(write_byte_signature);
  jit_type_free(write_word_signature);
  jit_type_free(update_timers_signature);
  jit_type_free(step_lcd_signature);
  jit_type_free(handle_events_signature);
  jit_type_free(throttle_signature);
}

long int dynarec_cpu::emulate(mem &m, keypad &k, lcd &l, sound &s, long int limit) {
  long int steps = 0;
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
        uint16_t start = pc.w;
        uint8_t op = m.read_byte(pc.w);
        switch(op) {
          #include "jumps.h"
          #include "conditionals.h"
          default: {
            cout << "ran into unimplemented jump or conditional branch" << endl;
            return 0;
          }
        }
        steps += 1;
        if (limit != 0) {
          if (steps >= limit) {
            delete storage;
            cout << "executed " << steps << " opcodes in the dynarec" << endl;
            return steps;
          }
        }
        uint16_t end = pc.w;
#ifdef DYNAREC_DEBUG
        cout << hex << (int)op << " mapped " << hex << (int)start << " to " << hex << (int)end << endl;
#endif
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
#ifdef DYNAREC_DEBUG
    cout << hex << (int)af.w << " " << hex << (int)bc.w << " " << hex << (int)de.w << " " << hex << (int)hl.w << endl;
    cout << "executing block " << idx.value() << endl;
#endif
    pc.w = storage->exec_block(idx.value());
    steps += storage->get_num_ops(idx.value());
    if (limit != 0) {
      if (steps >= limit) {
        delete storage;
        cout << "executed " << steps << " opcodes in the dynarec" << endl;
        return steps;
      }
    }
#ifdef DYNAREC_DEBUG
    cout << hex << (int)af.w << " " << hex << (int)bc.w << " " << hex << (int)de.w << " " << hex << (int)hl.w << endl;
#endif
  }
  delete storage;
  return 0;
}

/*
LibJIT's function destructors do not actually delete the underlying raw function until the context is destroyed
since we want to be able to replace blocks individually when the cache gets filled, this means that we need a JIT
context for each block. As a result, there is quite a bit of repetition for now
*/
optional<cache_block*> dynarec_cpu::translate(uint16_t address, mem &m, keypad &k, lcd &l) {
  //check if the translation is going to fail
  uint8_t opcode = m.read_byte(address);
  if (is_cond(opcode) || is_jump(opcode)) {
    return nullopt;
  }
  //we are sure the translation won't fail at this point
  //let's create a new context for this block
  jit_context *cx = new jit_context();
  //let's allocate memory for the new block here
  cache_block *block = new cache_block(*cx);

  location start = address;
  block->set_start(start);
  block->build_start();
  long int num_ops = 0;

  //get the addresses of the four auxilary classes
  //we have to do this for each block since they don't share a context
  jit_value m_addr = block->new_constant(&m, type_class_ptr);
  jit_value k_addr = block->new_constant(&k, type_class_ptr);
  jit_value l_addr = block->new_constant(&l, type_class_ptr);
  jit_value tp_addr = block->new_constant(&tp, type_class_ptr);

  //declare the most common JIT constants
  jit_value zero = block->new_constant(0, jit_type_ushort);
  jit_value one = block->new_constant(1, jit_type_ushort);
  jit_value two = block->new_constant(2, jit_type_ushort);
  jit_value four = block->new_constant(4, jit_type_ushort);
  jit_value seven = block->new_constant(7, jit_type_ushort);
  jit_value lower_bits = block->new_constant(0x0f, jit_type_ushort);
  jit_value upper_bits = block->new_constant(0xf0, jit_type_ushort);
  jit_value lower_byte = block->new_constant(0x00ff, jit_type_ushort);
  jit_value upper_byte = block->new_constant(0xff00, jit_type_ushort);
  jit_value f_z = block->new_constant(F_Z, jit_type_ushort);
  jit_value f_n = block->new_constant(F_N, jit_type_ushort);
  jit_value f_h = block->new_constant(F_H, jit_type_ushort);
  jit_value f_c = block->new_constant(F_C, jit_type_ushort);
  jit_value f_addr = block->new_constant(&af.b.l, type_uint8_t_ptr);
  jit_value a_addr = block->new_constant(&af.b.h, type_uint8_t_ptr);
  jit_value sp_addr = block->new_constant(&sp, type_uint16_t_ptr);

  //unused jit_values will be optimized away so define the majority here
  jit_value f_val, a_val, sp_val,
            reg8_addr, reg16_addr,
            reg8_val, reg16_val,
            condition, flag_val, not_flag, temp;

  //read in opcodes until we hit a conditional or jump
  //we are guaranteed to have at least one valid instruction due to the check at the beginning of this function
  while (!(is_cond(opcode)||is_jump(opcode))) {
    if (opcode == 0xcb) {
      address += 1;
      opcode = m.read_byte(address);
      switch(opcode) {
        #include "cb_translations.h"
        default: {
          cout << "ran into unimplemented 0xcb opcode:" << hex << (int)opcode << " at [" << hex << (int)address << "]" << endl;
          return nullopt;
        }
      }
    }
    else {
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
          return nullopt;
        }
      }
    }
    num_ops += 1;
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
  location end = address;
  block->set_end(end);
  block->set_num_ops(num_ops);
  block->insn_return();
  block->compile();
  block->bind();
  block->build_end();
  return block;
}
#endif
