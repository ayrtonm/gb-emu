#include <iostream>
#include <jit/jit-plus.h>
#include "dynarec.h"
#include "translate.h"

using namespace std;

dynarec_cpu::dynarec_cpu() {
  pc.w = 0x0100;
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
