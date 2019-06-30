#include <iostream>
#include <fstream>
#include <algorithm>
#include "keypad.h"

using namespace std;

keypad::keypad(string configfile) {
  if (configfile != "") {
    //read in configuration from file
    ifstream config;
    try {
      config.open(configfile, ifstream::in);
      if (!config.is_open()) {
        throw runtime_error("unable to open config file");
        return;
      }
      string option, value;
      while (config >> option >> value) {
        //check if config option is for setting a key
        if (option.compare(0, keyoption.length(), keyoption) == 0) {
          cout << "setting " << option << " to " << value << endl;
          keymap.emplace(value_to_keycode[value], option_to_key[option]);
          keyset.push_back(option_to_key[option]);
        }
      }
      config.close();
    }
    catch (const ifstream::failure &e) {
      cout << e.what() << endl;
      exit(2);
    }
  }
  for (vector<key>::iterator it = allkeys.begin(); it != allkeys.end(); it++) {
    //if key was not set in config file, set to its default value
    if (find(keyset.begin(), keyset.end(), *it) == keyset.end()) {
      keymap.emplace(default_keymap[*it],*it);
    }
  }
}

request keypad::handle_events(mem &m, double *sleep_factor) {
  while(SDL_PollEvent (&event)) {
    if (event.type == SDL_WINDOWEVENT) {
      switch (event.window.event) {
        case SDL_WINDOWEVENT_RESIZED: {
          if (event.window.data1 != 0 && event.window.data2 != 0) {
            return resize;
          }
        }
      }
    }
    //update memory but don't trigger interrupt on release
    else if (event.type == SDL_KEYUP) {
      if (keymap.find(event.key.keysym.sym) != keymap.end()) {
        switch(keymap[event.key.keysym.sym]) {
          case A: {
            if ((m.get_keys(special) & JOYP_A_BIT) == 0x00) {
              m.update_keys(special, JOYP_A_BIT, release);
            }
            break;
          }
          case B: {
            if ((m.get_keys(special) & JOYP_B_BIT) == 0x00) {
              m.update_keys(special, JOYP_B_BIT, release);
            }
            break;
          }
          case Start: {
            if ((m.get_keys(special) & JOYP_START_BIT) == 0x00) {
              m.update_keys(special, JOYP_START_BIT, release);
            }
            break;
          }
          case Select: {
            if ((m.get_keys(special) & JOYP_SELECT_BIT) == 0x00) {
              m.update_keys(special, JOYP_SELECT_BIT, release);
            }
            break;
          }
          case Up: {
            if ((m.get_keys(direction) & JOYP_U_BIT) == 0x00) {
              m.update_keys(direction, JOYP_U_BIT, release);
            }
            break;
          }
          case Down: {
            if ((m.get_keys(direction) & JOYP_D_BIT) == 0x00) {
              m.update_keys(direction, JOYP_D_BIT, release);
            }
            break;
          }
          case Left: {
            if ((m.get_keys(direction) & JOYP_L_BIT) == 0x00) {
              m.update_keys(direction, JOYP_L_BIT, release);
            }
            break;
          }
          case Right: {
            if ((m.get_keys(direction) & JOYP_R_BIT) == 0x00) {
              m.update_keys(direction, JOYP_R_BIT, release);
            }
            break;
          }
        }
      }
    }
    //update memory and trigger on interrupt
    else if (event.type == SDL_KEYDOWN) {
      if (keymap.find(event.key.keysym.sym) != keymap.end()) {
        switch(keymap[event.key.keysym.sym]) {
          case A: {
            if (m.get_keys(special) & JOYP_A_BIT) {
              m.update_keys(special, JOYP_A_BIT, press);
            if (m.get_keys_loaded() == special) {
              m.write_byte_internal(O_IO+IO_IR, m.read_byte(O_IO+IO_IR) | INT_JOY);
            }
            }
            break;
          }
          case B: {
            if (m.get_keys(special) & JOYP_B_BIT) {
              m.update_keys(special, JOYP_B_BIT, press);
            if (m.get_keys_loaded() == special) {
              m.write_byte_internal(O_IO+IO_IR, m.read_byte(O_IO+IO_IR) | INT_JOY);
            }
            }
            break;
          }
          case Start: {
            if (m.get_keys(special) & JOYP_START_BIT) {
              m.update_keys(special, JOYP_START_BIT, press);
            if (m.get_keys_loaded() == special) {
              m.write_byte_internal(O_IO+IO_IR, m.read_byte(O_IO+IO_IR) | INT_JOY);
            }
            }
            break;
          }
          case Select: {
            if (m.get_keys(special) & JOYP_SELECT_BIT) {
              m.update_keys(special, JOYP_SELECT_BIT, press);
            if (m.get_keys_loaded() == special) {
              m.write_byte_internal(O_IO+IO_IR, m.read_byte(O_IO+IO_IR) | INT_JOY);
            }
            }
            break;
          }
          case Up: {
            if (m.get_keys(direction) & JOYP_U_BIT) {
              m.update_keys(direction, JOYP_U_BIT, press);
            if (m.get_keys_loaded() == direction) {
              m.write_byte_internal(O_IO+IO_IR, m.read_byte(O_IO+IO_IR) | INT_JOY);
            }
            }
            break;
          }
          case Down: {
            if (m.get_keys(direction) & JOYP_D_BIT) {
              m.update_keys(direction, JOYP_D_BIT, press);
            if (m.get_keys_loaded() == direction) {
              m.write_byte_internal(O_IO+IO_IR, m.read_byte(O_IO+IO_IR) | INT_JOY);
            }
            }
            break;
          }
          case Left: {
            if (m.get_keys(direction) & JOYP_L_BIT) {
              m.update_keys(direction, JOYP_L_BIT, press);
            if (m.get_keys_loaded() == direction) {
              m.write_byte_internal(O_IO+IO_IR, m.read_byte(O_IO+IO_IR) | INT_JOY);
            }
            }
            break;
          }
          case Right: {
            if (m.get_keys(direction) & JOYP_R_BIT) {
              m.update_keys(direction, JOYP_R_BIT, press);
            if (m.get_keys_loaded() == direction) {
              m.write_byte_internal(O_IO+IO_IR, m.read_byte(O_IO+IO_IR) | INT_JOY);
            }
            }
            break;
          }
          case Quit: {
            if (m.get_dumpmemory()) {
              m.dump_memory();
            }
            m.dump_ram();
            return quit;
          }
#ifdef DEBUG
          case SDLK_p: {
            getchar();
            break;
          }
          case SDLK_h: {
            *(sleep_factor + 2) -= *(sleep_factor + 3);
            if (*(sleep_factor + 2) <= *(sleep_factor + 3)) {
              *(sleep_factor + 3) = *(sleep_factor + 2)/10.0;
            }
            cout << *sleep_factor << " " << *(sleep_factor + 1) << " " << *(sleep_factor + 2) << " " << *(sleep_factor + 3) <<  endl;
            break;
          }
          case SDLK_l: {
            *(sleep_factor + 2) += *(sleep_factor +3);
            if (*(sleep_factor + 2) >= *(sleep_factor + 3) * 100.0) {
              *(sleep_factor + 3) = *(sleep_factor + 2);
            }
            cout << *sleep_factor << " " << *(sleep_factor + 1) << " " << *(sleep_factor + 2) << " " << *(sleep_factor + 3) <<  endl;
            break;
          }
          case SDLK_j: {
            *sleep_factor -= *(sleep_factor + 1);
            if (*sleep_factor <= *(sleep_factor + 1)) {
              *(sleep_factor + 1) = *sleep_factor/10.0;
            }
            cout << *sleep_factor << " " << *(sleep_factor + 1) << " " << *(sleep_factor + 2) << " " << *(sleep_factor + 3) <<  endl;
            break;
          }
          case SDLK_k: {
            *sleep_factor += *(sleep_factor + 1);
            if (*sleep_factor >= *(sleep_factor + 1) * 100.0) {
              *(sleep_factor + 1) = *sleep_factor;
            }
            cout << *sleep_factor << " " << *(sleep_factor + 1) << " " << *(sleep_factor + 2) << " " << *(sleep_factor + 3) <<  endl;
            break;
          }
#endif
        }
      }
    }
  }
  return none;
};
