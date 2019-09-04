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
    cache_block() { valid = true; reg_eval = false; }
    ~cache_block();
    bool is_valid() { return valid; }
    void invalidate() { valid = false; }
    uint16 get_start() { return start_address; }
    void set_start(uint16 address) { start_address = address; }
    uint16 get_end() { return end_address; }
    void set_end(uint16 address) { end_address = address; }
    void exec();
    void store_data(uint8 data) { raw_data.push_back(data); }
  private:
    vector<uint8> raw_data;
    uint16 start_address, end_address;
    bool valid, reg_eval;
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
