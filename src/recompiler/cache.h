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
    uint16 get_start() {
      return start_address;
    }
    void set_start(uint16 address) {
      start_address = address;
    }
    uint16 get_end() {
      return end_address;
    }
    void set_end(uint16 address) {
      end_address = address;
    }
    void store_data(uint8 data) {
      raw_data.push_back(data);
    }
    uint8 get_last_byte() {
      return raw_data.back();
    }
    uint16 get_last_word() {
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
    vector<uint8> raw_data;
    uint16 start_address, end_address;
    bool valid;
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
    optional<int> find_block(uint16 start_address);
    uint16 exec_block(int idx);
    void invalidate_blocks(uint16 modified_address);
  private:
    //num_blocks refers to the number of valid blocks
    int num_blocks;
    std::array<cache_block*, MAX_BLOCKS> blocks;
    void insert_block_internal(cache_block *block, int idx);
};

#endif
