#include <iostream>
#include <algorithm> //find()
#include <vector>
#include <utility> //array
#include "translate.h"
#include "cache.h"
//figure out what parts of cpu.h need to be shared between the interpreter and recompiler then delete the following line
#include "../interpreter/cpu.h"
#include "../mem.h"
#include "../bits.h"

using namespace std;

bool is_cond(uint8 opcode) {
  return find(begin(cond_branches), end(cond_branches), opcode) != end(cond_branches);
}

bool is_jump(uint8 opcode) {
  return find(begin(jumps), end(jumps), opcode) != end(jumps);
}

cache_block translate(uint16 start_address, mem &m) {
  cache_block block;
  block.start_address = start_address;
  uint16 pc = start_address;
  for (uint8 op = m.read_byte(pc); !is_cond(op); op = m.read_byte(pc)) {
    //store opcode and its arguments
    for (int i = 0; i < length[op]; i++) {
      block.sequence.push_back(m.read_byte(pc + i));
    }
    pc += length[op];
    if (is_jump(op)) {
      //update pc
      switch(op) {
        case 0x18: {pc += (signed char)m.read_byte(pc-1);break;}
        case 0xc3: {pc = m.read_word(pc-2);break;}
        case 0xc7: {pc = 0x00;break;}
        //ret
        //case 0xc9: {break;}
        //remember to push the foreign program counter onto the native stack
        case 0xcd: {pc = m.read_word(pc-2);break;}
        case 0xcf: {pc = 0x08;break;}
        case 0xd7: {pc = 0x10;break;}
        //reti
        //case 0xd9: {break;}
        case 0xdf: {pc = 0x10;break;}
        case 0xe7: {pc = 0x20;break;}
        //jp [hl]
        //case 0xe9: {break;}
        case 0xef: {pc = 0x28;break;}
        case 0xf7: {pc = 0x30;break;}
        case 0xff: {pc = 0x38;break;}
        default: {cout << "opcode not implemented\n";break;}
      }
    }
  }
  block.end_address = pc;
  uint8 cond_op = m.read_byte(pc);
  pc += length[cond_op];
  block.if_true = pc;
  block.sequence.push_back(cond_op);
  switch(cond_op) {
    case 0x20: {block.if_true += (signed char)m.read_byte(pc-1);break;}
    case 0x28: {block.if_true += (signed char)m.read_byte(pc-1);break;}
    case 0x30: {block.if_true = (signed char)m.read_byte(pc-1);break;}
    case 0x38: {block.if_true = (signed char)m.read_byte(pc-1);break;}
    //cond ret
    //case 0xc0: {break;}
    case 0xc2: {block.if_true = pc-2;break;}
    case 0xc4: {block.if_true = pc-2;break;}
    //cond ret
    //case 0xc8: {break;}
    case 0xca: {block.if_true = pc-2;break;}
    case 0xcc: {block.if_true = pc-2;break;}
    //cond ret
    //case 0xd0: {break;}
    case 0xd2: {block.if_true = pc-2;break;}
    case 0xd4: {block.if_true = pc-2;break;}
    //cond ret
    //case 0xd8: {break;}
    case 0xda: {block.if_true = pc-2;break;}
    case 0xdc: {block.if_true = pc-2;break;}
  }
  block.if_false = pc;
  block.in_use = true;
  return block;
}

void print_cache_block(cache_block block) {
  for (vector<uint8>::iterator it = block.sequence.begin(); it != block.sequence.end(); ++it) {
    cout << "0x" << hex << (int)*it << endl;
  }
  cout << "starting address: " << hex << (int)block.start_address << endl;
  cout << "end address: " << hex << (int)block.end_address << endl;
  cout << "if condition is true, go to: " << hex << (int)block.if_true << endl;
  cout << "if condition is false, go to: " << hex << (int)block.if_false << endl << endl;
}
