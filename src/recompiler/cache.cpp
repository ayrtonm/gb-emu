#include "cache.h"

cache::cache() {
}

cache::~cache() {
  //iterate through the cache and free all used blocks
}

int cache::insert_block(cache_block *block) {
  bool evict_block = true;
  int idx;
  for (array<cache_block,MAX_BLOCKS>::iterator it = blocks.begin(); it != blocks.end(); it++) {
    if (!it->is_valid()) {
      //insert block here
      evict_block = false;
      idx = it-blocks.begin();
      break;
    }
  }
  if (evict_block) {
    //find last used block
    idx = find_last_used();
    //insert bloch here
  }
  return idx;
}

optional<int> cache::find_block(uint16 start_address) {
  return nullopt;
}

uint16 cache::exec_block(int idx) {
  return blocks[idx].get_end_address();
}

int cache::find_last_used() {
}
