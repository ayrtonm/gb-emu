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
  pc.w = 0x0100;
  type_uint8_ptr = jit_type_create_pointer(jit_type_ubyte, 0);
  type_uint16_ptr = jit_type_create_pointer(jit_type_ushort, 0);
  jit_type_t read_mem_arg[1];
  read_mem_arg[0] = jit_type_ushort;
  read_byte_signature = jit_type_create_signature(jit_abi_cdecl, jit_type_ubyte, read_mem_arg, 1, 1);
  read_word_signature = jit_type_create_signature(jit_abi_cdecl, jit_type_ushort, read_mem_arg, 1, 1);
}

dynarec_cpu::~dynarec_cpu() {
}

void dynarec_cpu::emulate(mem &m, keypad &k, lcd &l, sound &s) {
  //initialize libjit stuff
  jit_context context;

  cache *storage = new cache();
  optional<int> current_idx, next_idx;
  uint16 current_address = pc.w;
  uint16 next_address;

  cache_block block = translate(current_address,m,&context);
  current_idx = storage->insert_block(block);

  for (;;) {
    //load libjit registers from cpu state
    next_address = storage->exec_block(current_idx.value());
    //store libjit registers in cpu state
    next_idx = storage->find_block(next_address);
    //if find_block fails translate the next block
    if (!next_idx) {
      block = translate(current_address,m,&context);
      next_idx = storage->insert_block(block);
    }
    current_idx = next_idx;
    current_address = next_address;
  }
  delete storage;
}

cache_block dynarec_cpu::translate(uint16 address, mem &m, jit_context *context) {
  cache_block block(*context);
  context->build_start();
  block.set_start(address);
  //make a jit_value constant that contains a pointer to m.read_word()
  //add a call to m.read_word() using this pointer to the block's function
  //jit_value read_word = block.new_constant(m, type_mem_ptr);
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
      #include "instructions.h"
    }
    address++;
    opcode = m.read_byte(address);
  };
  block.set_end(address);
  block.compile();
  context->build_end();
  block.bind();
  return block;
}
