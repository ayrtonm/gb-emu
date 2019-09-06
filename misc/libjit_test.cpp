#include <iostream>
#include <jit/jit-plus.h>

int reg = 1;
jit_type_t type_int_ptr = jit_type_create_pointer(jit_type_int, 0);

int increment_reg() {
  reg++;
  return reg;
}

class myblock : public jit_function {
  public:
    myblock(jit_context &context) : jit_function(context) {
      create();
      set_recompilable();
    }
    virtual void build();
  protected:
    virtual jit_type_t create_signature();
};

void myblock::build() {
  jit_value constant = new_constant(2, jit_type_int);
  jit_value addr = new_constant(&reg, type_int_ptr);
  jit_value r0 = insn_load_relative(addr, 0, jit_type_int);
  insn_store_relative(addr,0,constant * r0);
  jit_value r1 = insn_call_native(NULL, (void *)&increment_reg, signature(), NULL, 0, 0);
  insn_return(r1);
}

jit_type_t myblock::create_signature() {
  return signature_helper(jit_type_int,end_params); 
}
int main(int argc, char *argv[]) {
  jit_context context;
  myblock func(context);
  context.build_start();
  func.build();
  func.compile();

  typedef int (*FF)(void);
  FF closure = (FF)func.closure();
  context.build_end();
  for (int i = 0; i < 10; i++) {
    std::cout << closure() << " ";
    std::cout << reg << std::endl;
  }
}
