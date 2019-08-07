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

cache_block *translate(uint16 start_address) {
}
