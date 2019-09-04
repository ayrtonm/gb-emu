#include "cache.h"

cache_block::cache_block() {
  valid = true;
}

cache_block::~cache_block() {
}

bool cache_block::is_valid() {
  return valid;
}

void cache_block::invalidate() {
  valid = false;
}

uint16 cache_block::exec() {
  //run code
  //evaluate register state
  bool jump;
  if (jump) {
    return get_true();
  }
  else {
    return get_false();
  }
}
