#ifndef CACHE_H
#define CACHE_H
#define MAX_BLOCKS 32
#include <vector>
#include <utility>
#include "../bits.h"

using namespace std;

typedef struct cache_block {
  vector<uint8> sequence;
  //end_address and if_false are redundant
  uint16 start_address, end_address, if_true, if_false;
  bool in_use = false;
} cache_block;

extern pair<bool, int> in_cache(uint16 start_address, cache_block *cache);

#endif
