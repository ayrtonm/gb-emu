#include <iostream>
#include <string>
#include <argp.h>
//#include "recompiler/dynarec.h"
#include "interpreter/cpu.h"
#include "mem.h"

struct arguments {
  char *args[2];
  bool gameloaded = false;
  bool configloaded = false;
  bool dumpmemory = false;
  bool recompiled = false;
  char *inputfile, *memorydumpfile, *configfile;
};

static struct argp_option options[] = {
  {"inputfile", 'l', "INPUTFILE", 0, "load the given file and start the emulator"},
  {"configfile", 'c',"CONFIGFILE", 0, "load the given configuration file"},
  {"memorydump", 'd', "OUTPUTFILE", 0, "print addresses and values of non-zero memory into OUTFILE at the end of emulation"},
  {NULL, 'r', NULL, 0, "use dynamic recompilation"},
  {0}
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
  struct arguments *arguments = (struct arguments *)state->input;
  switch(key) {
    case 'l':
      arguments->inputfile = arg;
      arguments->gameloaded = true;
      break;
    case 'd':
      arguments->memorydumpfile = arg;
      arguments->dumpmemory = true;
      break;
    case 'r':
      arguments->recompiled = true;
      break;
    case 'c':
      arguments->configfile = arg;
      arguments->configloaded = true;
      break;
    case ARGP_KEY_END:
      if (!arguments->gameloaded) {
        argp_usage(state);
      }
      break;
    default:
      return ARGP_ERR_UNKNOWN;
  }
  return 0;
};

static char args_doc[] = "-l FILENAME";
static char doc[] = "Gameboy Emulator";

static struct argp argp = {options, parse_opt, args_doc, doc};

int main(int argc, char *argv[]) {
  struct arguments arguments;
  argp_parse(&argp, argc, argv, 0, 0, &arguments);
  //get filename
  string filename(arguments.inputfile);
  //pointers to classes for emulated subsystems
  mem *m;
  keypad *k;
  //if were going to dump memory to a file on exit, store that files name in the mem class
  if (arguments.dumpmemory) {
    string memorydumpfile(arguments.memorydumpfile);
    m = new mem(filename, memorydumpfile);
  }
  //otherwise initialize mem class like normal
  else {
    m = new mem(filename);
  }
  if (arguments.configloaded) {
    string configfile(arguments.configfile);
    k = new keypad(configfile);
  }
  else {
    k = new keypad();
  }
  //if we are not dynamically recompiling, make a pointer to an instance of the cpu class and start emulator
  if (!arguments.recompiled) {
    cpu *c = new cpu;
    c->emulate(*m,*k);
    delete c;
  }
  //otherwise pass control to dynarec loop
  else {
    //dynamic_recompile_loop(*m);
  }
  delete m;
  return 0;
}
