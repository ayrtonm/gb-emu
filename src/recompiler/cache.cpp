#ifdef DYNAREC_CPU
#include "cache.h"
#include <iostream> //for debugging only
#include <algorithm>

int cache::insert_block(cache_block *block) {
  //if the cache is not full of valid blocks
  if (num_blocks < MAX_BLOCKS) {
    //find the first NULL pointer or invalid block
    for (auto it = blocks.begin(); it != blocks.end(); it++) {
      //if dereferenced iterator points to nothing
      if (*it == NULL) {
        int idx = it - blocks.begin();
        blocks[idx] = block;
        num_blocks += 1;
        return idx;
      }
      else if (!(*it)->is_valid()) {
        //this destroys the JIT context and frees the raw function
        int idx = it - blocks.begin();
        if (blocks[idx] != NULL) {
          delete blocks[idx];
        }
        blocks[idx] = block;
        num_blocks += 1;
        return idx;
      }
    }
  }
  //if the cache is full of valid blocks, replace the first block
  //this can be improved by keeping track of block priorities and replacing the last used block
  delete blocks[0];
  blocks[0] = block;
  return 0;
}

optional<int> cache::find_block(uint16 start_address) {
  for (auto it = blocks.begin(); it != blocks.end(); it++) {
    if (*it == NULL) {
      continue;
    }
    if ((*it)->get_start() == start_address) {
      return it - blocks.begin();
    }
  }
  return nullopt;
}

void cache::invalidate_blocks(uint16 modified_address) {
  for (auto it = blocks.begin(); it != blocks.end(); it++) {
    if ((modified_address >= (*it)->get_start()) && (modified_address <= (*it)->get_end())) {
      (*it)->invalidate();
      num_blocks -= 1;
    }
  }
}

uint16 cache::exec_block(int idx) {
  blocks[idx]->exec();
  return blocks[idx]->get_end();
}
#endif
