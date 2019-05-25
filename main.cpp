#include <iostream>
#include <string>
#include <argp.h>
#include "cpu.h"
#include "mem.h"
#include "lcd.h"

struct arguments {
  char *args[2];
  bool gameloaded = false;
  bool dumpmemory = false;
  char *inputfile, *memorydumpfile;
};

static struct argp_option options[] = {
  {"inputfile", 'l', "FILENAME", 0, "load the given file and start the emulator"},
  {"memorydump", 'd', "OUTFILE", 0, "print addresses and values of non-zero memory into OUTFILE at the end of emulation"},
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
    //case ARGP_KEY_ARG:
    //  if(state->arg_num >= 2) {
    //    argp_usage(state);
    //  }
    //  arguments->args[state->arg_num] = arg;
    //  break;
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
  string filename(arguments.inputfile);
  cpu *c;
  mem *m;
  c = new cpu;
  if (arguments.dumpmemory) {
    string memorydumpfile(arguments.memorydumpfile);
    m = new mem(filename, memorydumpfile);
  }
  else {
    m = new mem(filename);
  }
  c->emulate(*m);
  delete c;
  delete m;
  return 0;
}
