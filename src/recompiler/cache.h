#ifndef CACHE_H
#define CACHE_H
#define MAX_BLOCKS 32
#include <vector>
#include <array>
#include <utility>
#include <optional>
#include "../bits.h"

using namespace std;

class cache_block {
  public:
    cache_block();
    ~cache_block();
    bool is_valid();
    uint16 get_start_address() {
      return start_address;
    }
    uint16 get_end_address() {
      return end_address;
    }
  private:
    vector<uint8> raw_data;
    uint16 start_address, end_address, address_if_true, address_if_false;
    bool valid = false;
};

class cache {
  public:
    cache();
    ~cache();
    int insert_block(cache_block *block);
    uint16 exec_block(int idx);
    optional<int> find_block(uint16 start_address);
    int find_last_used();
  private:
    array<cache_block,MAX_BLOCKS> blocks;
};

#endif
