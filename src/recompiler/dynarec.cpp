#include <iostream>
#include "dynarec.h"
#include "translate.h"

using namespace std;

dynarec_cpu::dynarec_cpu() {
  pc.w = 0x0100;
}
dynarec_cpu::~dynarec_cpu() {
}
void dynarec_cpu::emulate(mem &m, keypad &k, lcd &l, sound &s) {
  //cache_block *b = new cache_block();
  //b->store_data(0xff);
  //b->store_data(0xfe);
  //b->store_data(0x00);
  //storage->insert_block(b);
  cache *storage = new cache();
  optional<int> current_idx, next_idx;
  uint16 current_address = pc.w;
  uint16 next_address;

  cache_block *block = translate(current_address,m);
  current_idx = storage->insert_block(block);

  for (;;) {
    next_address = storage->exec_block(current_idx.value());
    next_idx = storage->find_block(next_address);
    //if find_block fails translate the next block
    if (!next_idx) {
      block = translate(current_address,m);
      next_idx = storage->insert_block(block);
    }
    current_idx = next_idx;
    current_address = next_address;
  }
  delete storage;
}
