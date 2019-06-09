#include "cache.h"

pair<bool, int> in_cache(uint16 start_address, cache_block *cache) {
  for (int i = 0; i < MAX_BLOCKS; i++) {
    if (cache[i].start_address == start_address && cache[i].in_use) {
      return make_pair(true, i);
    }
  }
  //index outside cache if requested start address is not found in cache
  return make_pair(false, MAX_BLOCKS+1);
}
