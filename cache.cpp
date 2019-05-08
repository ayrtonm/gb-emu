#include "cache.h"

bool in_cache(uint16 start_address, cache_block *cache) {
  for (int i = 0; i < MAX_BLOCKS; i++) {
    if (cache[i].start_address == start_address && cache[i].in_use) {
      return true;
    }
  }
  return false;
}
