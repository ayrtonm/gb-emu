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
//placeholder until I figure out what the target functions take as arguments and return
//tells the compiler how to interpret the address of the entry point of the JIT-ed code
typedef int (*FF)(int);

class target_function : public jit_function {
  public:
    target_function(jit_context &context) : jit_function(context) {
      create();
      set_recompilable();
    }
    virtual void build();
  protected:
    virtual jit_type_t create_signature();
};

class cache_block {
  public:
    cache_block() { valid = true; reg_eval = false; }
    cache_block(target_function *func) { valid = true; reg_eval = false; function = func; }
    ~cache_block() { delete function; }
    bool is_valid() { return valid; }
    void invalidate() { valid = false; }
    uint16 get_start() { return start_address; }
    void set_start(uint16 address) { start_address = address; }
    uint16 get_end() { return end_address; }
    void set_end(uint16 address) { end_address = address; }
    void store_data(uint8 data) { raw_data.push_back(data); }
    void build() { function->build(); }
    void compile() { function->compile(); }
    void bind() { closure = (FF)function->closure(); }
    //0 is a placeholder for the argument
    void exec() { closure(0); }
  private:
    vector<uint8> raw_data;
    target_function *function;
    FF closure;
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
