#include <iostream>
#include "dynarec.h"
#include "translate.h"
#include "mem.h"

using namespace std;

int dynamic_recompile_loop(mem &m) {
  cache_block cache[MAX_BLOCKS];
  //foreign program counter
  uint16 pc = 0x0100;
  int i = 0;
  for (;;) {
    //check if a block with our desired start address is already in the cache
    pair<bool, int> test = in_cache(pc, cache);
    //if block not found
    if (!test.first) {
      //go through memory and translate opcodes into a cache block to store in the cache
      cache[i] = translate(pc, m);
      //emit IR from cache[i].sequence
    }
    //execute IR from cache[i]
    //evaluate cache[i] branch condition
    //update cache index i to next empty spot
    //if cache is full set index i to last used block
    //update pc based on previously evaluated branch condition
    //repeat
  }
}

int test_dynamic_recompile_loop(mem &m) {
  cache_block cache[MAX_BLOCKS];
  cache[0] = translate(0x0100, m);
  print_cache_block(cache[0]);

  cache[1] = translate(cache[0].if_true, m);
  cache[2] = translate(cache[0].if_false, m);
  print_cache_block(cache[1]);
  print_cache_block(cache[2]);
  cout << (in_cache(0x0100, cache).first ? "address 0x0100 is in cache" : "address 0x0100 is not in cache") << endl;
  return 0;
}
