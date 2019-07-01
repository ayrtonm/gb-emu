#include <iostream>
#include <jit/jit-plus.h>

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
  jit_value x = get_param(0);
  jit_value y = get_param(1);
  jit_value z = get_param(2);
  insn_return(x * y + z);
}

jit_type_t myblock::create_signature() {
  return signature_helper(jit_type_int,jit_type_int,jit_type_int,
                          jit_type_int,end_params); 
}
int main(int argc, char *argv[]) {
  jit_context context;
  //context.build_start();
  myblock func(context);
  func.build();
  func.compile();
  //context.build_end();

  //jit_int arg1, arg2, arg3;
  int arg1, arg2, arg3;
  void *args[3];
  //jit_int result;
  int result;
  arg1 = 3;
  arg2 = 5;
  arg3 = 2;
  args[0] = &arg1;
  args[1] = &arg2;
  args[2] = &arg3;
  typedef int (*FF)(int, int, int);
  FF closure = (FF)func.closure();
  for (int i = 0; i < 10000000; i++) {
    //func.apply(args, &result);
    result = closure(arg1, arg2, arg3);
  }
  std::cout << result << std::endl;
}
