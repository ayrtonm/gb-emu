#include <iostream>
#include <string>
#include <argp.h>
#include "mem.h"
#include "interpreter/cpu.h"
#ifdef DYNAREC_CPU
#include "recompiler/dynarec.h"
#endif

struct arguments {
  char *args[2];
  bool gameloaded = false;
  bool configloaded = false;
  bool dumpmemory = false;
  bool recompiled = false;
  bool saveloaded = false;
  char *inputfile, *memorydumpfile, *configfile, *savefile;
};

static struct argp_option options[] = {
  {"inputfile", 'l', "INPUTFILE", 0, "load the given file and start the emulator"},
  {"configfile", 'c',"CONFIGFILE", 0, "load the given configuration file"},
  {"memorydump", 'd', "OUTPUTFILE", 0, "print addresses and values of non-zero memory into OUTFILE at the end of emulation"},
  {"savefile", 's', "SAVEFILE", 0, "load and save external RAM to the following file"},
#ifdef DYNAREC_CPU
  {NULL, 'r', NULL, 0, "use dynamic recompilation"},
#endif
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
    case 's':
      arguments->savefile = arg;
      arguments->saveloaded = true;
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
  lcd *l;
  sound *s;
  //if were going to dump memory to a file on exit, store that files name in the mem class
  string memorydumpfile = (arguments.dumpmemory ? arguments.memorydumpfile : "");
  string configfile = (arguments.configloaded ? arguments.configfile : "");
  string savefile = (arguments.saveloaded ? arguments.savefile : "");
  m = new mem(filename, memorydumpfile, savefile);
  k = new keypad(configfile);
  l = new lcd(configfile);
  s = new sound(configfile);
#ifdef DYNAREC_CPU
  //if we are not dynamically recompiling, make a pointer to an instance of the cpu class and start emulator
  if (!arguments.recompiled) {
#endif
    cpu *c = new cpu;
    c->emulate(*m,*k,*l,*s);
    delete c;
#ifdef DYNAREC_CPU
  }
  //otherwise pass control to dynarec loop
  else {
    dynarec_cpu *c = new dynarec_cpu;
    c->emulate(*m,*k,*l,*s);
    delete c;
  }
#endif
  delete m;
  delete k;
  delete l;
  delete s;
  return 0;
}
