#include "cache.h"

void target_function::build() {
  jit_value x = get_param(0);
  insn_return(x);
}

jit_type_t target_function::create_signature() {
  return signature_helper(jit_type_int, jit_type_int, end_params);
}
