#include <iostream>
#include "dynarec.h"
#include "traverse.h"
#include "mem.h"

using namespace std;

int dynamic_recompile_loop(mem &m) {
  cache_block cache[MAX_BLOCKS];
  cache[0] = traverse(0x0100, m);
  print_cache_block(cache[0]);
  cout << (in_cache(0x0100, cache) ? "address 0x0100 is in cache" : "address 0x0100 is not in cache") << endl;
  return 0;
}
