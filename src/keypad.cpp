#include "keypad.h"

request keypad::handle_events(mem &m) {
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
      switch(event.key.keysym.sym) {
        case SDLK_z: {
          if ((m.get_keys(special) & JOYP_A_BIT) == 0x00) {
            m.update_keys(special, JOYP_A_BIT, up);
          }
          break;
        }
        case SDLK_x: {
          if ((m.get_keys(special) & JOYP_B_BIT) == 0x00) {
            m.update_keys(special, JOYP_B_BIT, up);
          }
          break;
        }
        case SDLK_RETURN: {
          if ((m.get_keys(special) & JOYP_START_BIT) == 0x00) {
            m.update_keys(special, JOYP_START_BIT, up);
          }
          break;
        }
        case SDLK_BACKSPACE: {
          if ((m.get_keys(special) & JOYP_SELECT_BIT) == 0x00) {
            m.update_keys(special, JOYP_SELECT_BIT, up);
          }
          break;
        }
        case SDLK_UP: {
          if ((m.get_keys(direction) & JOYP_U_BIT) == 0x00) {
            m.update_keys(direction, JOYP_U_BIT, up);
          }
          break;
        }
        case SDLK_DOWN: {
          if ((m.get_keys(direction) & JOYP_D_BIT) == 0x00) {
            m.update_keys(direction, JOYP_D_BIT, up);
          }
          break;
        }
        case SDLK_LEFT: {
          if ((m.get_keys(direction) & JOYP_L_BIT) == 0x00) {
            m.update_keys(direction, JOYP_L_BIT, up);
          }
          break;
        }
        case SDLK_RIGHT: {
          if ((m.get_keys(direction) & JOYP_R_BIT) == 0x00) {
            m.update_keys(direction, JOYP_R_BIT, up);
          }
          break;
        }
      }
    }
    //update memory and trigger on interrupt
    else if (event.type == SDL_KEYDOWN) {
      switch(event.key.keysym.sym) {
        case SDLK_z: {
          if (m.get_keys(special) & JOYP_A_BIT) {
            m.update_keys(special, JOYP_A_BIT, down);
          if (m.get_keys_loaded() == special) {
            m.write_byte_internal(O_IO+IO_IR, m.read_byte(O_IO+IO_IR) | INT_JOY);
          }
          }
          break;
        }
        case SDLK_x: {
          if (m.get_keys(special) & JOYP_B_BIT) {
            m.update_keys(special, JOYP_B_BIT, down);
          if (m.get_keys_loaded() == special) {
            m.write_byte_internal(O_IO+IO_IR, m.read_byte(O_IO+IO_IR) | INT_JOY);
          }
          }
          break;
        }
        case SDLK_RETURN: {
          if (m.get_keys(special) & JOYP_START_BIT) {
            m.update_keys(special, JOYP_START_BIT, down);
          if (m.get_keys_loaded() == special) {
            m.write_byte_internal(O_IO+IO_IR, m.read_byte(O_IO+IO_IR) | INT_JOY);
          }
          }
          break;
        }
        case SDLK_BACKSPACE: {
          if (m.get_keys(special) & JOYP_SELECT_BIT) {
            m.update_keys(special, JOYP_SELECT_BIT, down);
          if (m.get_keys_loaded() == special) {
            m.write_byte_internal(O_IO+IO_IR, m.read_byte(O_IO+IO_IR) | INT_JOY);
          }
          }
          break;
        }
        case SDLK_UP: {
          if (m.get_keys(direction) & JOYP_U_BIT) {
            m.update_keys(direction, JOYP_U_BIT, down);
          if (m.get_keys_loaded() == direction) {
            m.write_byte_internal(O_IO+IO_IR, m.read_byte(O_IO+IO_IR) | INT_JOY);
          }
          }
          break;
        }
        case SDLK_DOWN: {
          if (m.get_keys(direction) & JOYP_D_BIT) {
            m.update_keys(direction, JOYP_D_BIT, down);
          if (m.get_keys_loaded() == direction) {
            m.write_byte_internal(O_IO+IO_IR, m.read_byte(O_IO+IO_IR) | INT_JOY);
          }
          }
          break;
        }
        case SDLK_LEFT: {
          if (m.get_keys(direction) & JOYP_L_BIT) {
            m.update_keys(direction, JOYP_L_BIT, down);
          if (m.get_keys_loaded() == direction) {
            m.write_byte_internal(O_IO+IO_IR, m.read_byte(O_IO+IO_IR) | INT_JOY);
          }
          }
          break;
        }
        case SDLK_RIGHT: {
          if (m.get_keys(direction) & JOYP_R_BIT) {
            m.update_keys(direction, JOYP_R_BIT, down);
          if (m.get_keys_loaded() == direction) {
            m.write_byte_internal(O_IO+IO_IR, m.read_byte(O_IO+IO_IR) | INT_JOY);
          }
          }
          break;
        }
        case SDLK_q: {
          if (m.get_dumpmemory()) {
            m.dump_memory();
          }
          return quit;
        }
        case SDLK_p: {
          getchar();
        }
      }
    }
  }
  return none;
};
