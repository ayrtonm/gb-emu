#ifndef CACHE_H
#define CACHE_H
#define MAX_BLOCKS 32
#include <vector>
#include <array>
#include <utility>
#include <optional>
#include <jit/jit-plus.h>
#include "../bits.h"

using namespace std;

typedef struct location {
  uint16_t address;
  int bank;
} location;

class cache_block : public jit_function {
  //this typedef is a placeholder until I figure out what the target functions take as
  //arguments and return tells the compiler how to interpret the address of the entry
  //point of the JIT-ed code
  typedef void (*function)(void);
  public:
    cache_block(jit_context& cx) : jit_function(cx), context(&cx), valid(true) {
      create();
      set_recompilable();
    }
    ~cache_block() {
      delete context;
    };
    bool is_valid() {
      return valid;
    }
    void invalidate() {
      valid = false;
    }
    location get_start() {
      return start;
    }
    void set_start(location st) {
      start = st;
    }
    location get_end() {
      return end;
    }
    void set_end(location en) {
      end = en;
    }
    long int get_num_ops() {
      return num_ops;
    }
    void set_num_ops(long int num) {
      num_ops = num;
    }
    void store_data(uint8_t data) {
      raw_data.push_back(data);
    }
    uint8_t get_last_byte() {
      return raw_data.back();
    }
    uint16_t get_last_word() {
      return (raw_data.back() << 8) + (raw_data[raw_data.size()-2]);
    }
    void bind() {
      function_name = (function)closure();
    }
    void exec() {
      function_name();
    }
    void build_start() {
      context->build_start();
    }
    void build_end() {
      context->build_end();
    }
  protected:
    virtual jit_type_t create_signature() {
      return signature_helper(jit_type_int, end_params);
    }
  private:
    jit_context *context;
    function function_name;
    vector<uint8_t> raw_data;
    location start, end;
    bool valid;
    long int num_ops;
};

class cache {
  public:
    cache() : num_blocks(0) {
      for (auto it = blocks.begin(); it != blocks.end(); it++) {
        *it = NULL;
      }
    };
    ~cache() {
      for (auto it = blocks.begin(); it != blocks.end(); it++) {
        if (*it != NULL) {
          delete *it;
        }
      }
    };
    int insert_block(cache_block *block);
    optional<int> find_block(uint16_t start_address);
    uint16_t exec_block(int idx);
    void invalidate_blocks(uint16_t modified_address);
    long int get_num_ops(int idx);
  private:
    //num_blocks refers to the number of valid blocks
    int num_blocks;
    std::array<cache_block*, MAX_BLOCKS> blocks;
    void insert_block_internal(cache_block *block, int idx);
};

#endif
