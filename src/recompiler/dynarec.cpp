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
  jit_type_t read_mem_arg[] = {jit_type_void_ptr, jit_type_ushort};
  jit_type_t write_byte_args[] = {jit_type_void_ptr, jit_type_ushort, jit_type_ubyte};
  jit_type_t write_word_args[] = {jit_type_void_ptr, jit_type_ushort, jit_type_ushort};
  read_byte_signature = jit_type_create_signature(jit_abi_cdecl, jit_type_ubyte, read_mem_arg, 2, 1);
  read_word_signature = jit_type_create_signature(jit_abi_cdecl, jit_type_ushort, read_mem_arg, 2, 1);
  write_byte_signature = jit_type_create_signature(jit_abi_cdecl, jit_type_void, write_byte_args, 3, 1);
  write_word_signature = jit_type_create_signature(jit_abi_cdecl, jit_type_void, write_word_args, 3, 1);
}

dynarec_cpu::~dynarec_cpu() {
}

void dynarec_cpu::emulate(mem &m, keypad &k, lcd &l, sound &s) {
  //initialize libjit stuff
  jit_context context;

  cache *storage = new cache();
  optional<int> idx;
  uint16 address = pc.w;
  cache_block block(context);

  for (;;) {
    idx = storage->find_block(address);
    if (!idx) {
      block = translate(address, m, &context);
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
          block = translate(address, m, &context);
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

cache_block dynarec_cpu::translate(uint16 address, mem &m, jit_context *context) {
  cache_block block(*context);
  context->build_start();
  block.set_start(address);
  jit_value m_addr = block.new_constant(&m, type_class_ptr);
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
