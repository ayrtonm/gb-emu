#include <iostream>
#include <string>
#include "globals.h"

void print_help(void)
{
  using namespace std;
  cout << "Gameboy Emulator\n";
  cout << "Usage:\n";
  cout << "  -h\t\t\tprint this help text\n";
  cout << "  -r  [filename]\tload the given file and start the emulator\n";
}

int main(int argc, char *argv[])
{
  if (argc <= 2)
  {
    print_help();
    return 0;
  }
  string filename = argv[2];
  cpu *c;
  mem *m;
  lcd *l;
  c = new cpu;
  m = new mem(filename);
  l = new lcd;
  c->emulate(*m, *l);
  return 0;
}
