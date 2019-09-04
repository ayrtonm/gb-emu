#include <iostream>
#include <algorithm> //find()
#include <vector>
#include <utility> //array
#include "translate.h"
#include "cache.h"
#include "../mem.h"
#include "../bits.h"

using namespace std;

bool is_cond(uint8 opcode) {
  return find(begin(cond_branches), end(cond_branches), opcode) != end(cond_branches);
}

bool is_jump(uint8 opcode) {
  return find(begin(jumps), end(jumps), opcode) != end(jumps);
}

cache_block *translate(uint16 address, mem &m) {
  cache_block *block = new cache_block();
  uint8 opcode;
  do {
    opcode = m.read_byte(address);
    block->store_data(opcode);
    //store opcode arguments if any exist
    for (int i = 1; i < length[opcode]; i++) {
      address += 1;
      block->store_data(m.read_byte(address));
    }
    //handle unconditional jump instruction
    if (is_jump(opcode)) {
      switch(opcode) {
        #include "jumps.h"
      }
    }
    else {
      address++;
    }
  } while (!is_cond(opcode));
  return block;
}
