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
    void invalidate();
    uint16 get_start() {
      return start_address;
    }
    uint16 get_end() {
      return end_address;
    }
    uint16 get_true() {
      return address_if_true;
    }
    uint16 get_false() {
      return address_if_false;
    }
    uint16 exec();
    void store_data(uint8 data) {
      raw_data.push_back(data);
    }
  private:
    vector<uint8> raw_data;
    uint16 start_address, end_address, address_if_true, address_if_false;
    bool valid;
};

class cache {
  public:
    cache();
    ~cache();
    int insert_block(cache_block *block);
    optional<int> find_block(uint16 start_address);
    uint16 exec_block(int idx);
    int find_last_used();
    void invalidate_blocks(uint16 modified_address);
  private:
    array<cache_block,MAX_BLOCKS> blocks;
    array<int,MAX_BLOCKS> priorities;
};

#endif
