#include "cache.h"
#include <algorithm>

cache::cache() {
  fill(priorities.begin(),priorities.end(),0);
}

cache::~cache() {
  //iterate through the cache and free all used blocks
  //vector<cache_block>::iterator it, jt;
  //it = blocks.begin();
  //while (it != blocks.end()) {
  //  jt = it;
  //  it++;
  //  delete jt;
  //}
}

int cache::insert_block(cache_block block) {
  if (blocks.size() < MAX_BLOCKS) {
    blocks.push_back(block);
    priorities.push_back(0);
    return blocks.size()-1;
  }
  bool evict_block = true;
  int idx;
  for (vector<cache_block>::iterator it = blocks.begin(); it != blocks.end(); it++) {
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
  for (vector<cache_block>::iterator it = blocks.begin(); it != blocks.end(); it++) {
    if (it->get_start() == start_address) {
      return it-blocks.begin();
    }
  }
  return nullopt;
}

int cache::find_last_used() {
  return *max_element(priorities.begin(), priorities.end());
}

void cache::invalidate_blocks(uint16 modified_address) {
  for (vector<cache_block>::iterator it = blocks.begin(); it != blocks.end(); it++) {
    if ((modified_address >= it->get_start()) && (modified_address <= it->get_end())) {
      it->invalidate();
    }
  }
}

uint16 cache::exec_block(int idx) {
  priorities[idx] = *max_element(priorities.begin(), priorities.end()) + 1;
  if (*max_element(priorities.begin(), priorities.end()) == MAX_BLOCKS) {
    for (vector<int>::iterator it = priorities.begin(); it != priorities.end(); it++) {
      *it -= MAX_BLOCKS;
    }
  }
  blocks[idx].exec();
  //handle jump or conditional branch
  //return next address
  return 0; 
}
