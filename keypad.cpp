#include "keypad.h"

int keypad::handle_events(mem &m) {
  while(SDL_PollEvent (&event)) {
    if (event.type == SDL_WINDOWEVENT) {
      switch (event.window.event) {
        case SDL_WINDOWEVENT_RESIZED: {
          //if (event.window.data1 != 0 && event.window.data2 != 0)
          //{
          //  int w, h;
          //  SDL_GetWindowSize(window, &w, &h);
          //  scale = MIN((float)w/160.0,(float)h/144.0);
          //  offset.x = (w - (160*scale))/2;
          //  offset.y = (h - (144*scale))/2;
          //  offset.w = 160*scale;
          //  offset.h = 144*scale;
          //  SDL_RenderClear(renderer);
          //  SDL_UpdateTexture(screen, NULL, &pixels[0], 160*4);
          //  SDL_RenderCopy(renderer, screen, NULL, &offset);
          //  SDL_RenderPresent(renderer);
          //}
          break;
        }
      }
    }
    //update memory but don't trigger interrupt on release
    else if (event.type == SDL_KEYUP) {
      switch(event.key.keysym.sym) {
        case SDLK_z: {
          if ((m.get_keys(special) & JOYP_A_BIT) == 0x00) {
            m.update_keys(special, JOYP_A_BIT, false);
          }
          break;
        }
        case SDLK_x: {
          if ((m.get_keys(special) & JOYP_B_BIT) == 0x00) {
            m.update_keys(special, JOYP_B_BIT, false);
          }
          break;
        }
        case SDLK_RETURN: {
          if ((m.get_keys(special) & JOYP_START_BIT) == 0x00) {
            m.update_keys(special, JOYP_START_BIT, false);
          }
          break;
        }
        case SDLK_BACKSPACE: {
          if ((m.get_keys(special) & JOYP_SELECT_BIT) == 0x00) {
            m.update_keys(special, JOYP_SELECT_BIT, false);
          }
          break;
        }
        case SDLK_UP: {
          if ((m.get_keys(direction) & JOYP_U_BIT) == 0x00) {
            m.update_keys(direction, JOYP_U_BIT, false);
          }
          break;
        }
        case SDLK_DOWN: {
          if ((m.get_keys(direction) & JOYP_D_BIT) == 0x00) {
            m.update_keys(direction, JOYP_D_BIT, false);
          }
          break;
        }
        case SDLK_LEFT: {
          if ((m.get_keys(direction) & JOYP_L_BIT) == 0x00) {
            m.update_keys(direction, JOYP_L_BIT, false);
          }
          break;
        }
        case SDLK_RIGHT: {
          if ((m.get_keys(direction) & JOYP_R_BIT) == 0x00) {
            m.update_keys(direction, JOYP_R_BIT, false);
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
            m.update_keys(special, JOYP_A_BIT, true);
          if (m.get_keys_loaded() == special) {
            m.write_byte_internal(O_IO+IO_IR, m.read_byte(O_IO+IO_IR) | INT_JOY);
          }
          }
          break;
        }
        case SDLK_x: {
          if (m.get_keys(special) & JOYP_B_BIT) {
            m.update_keys(special, JOYP_B_BIT, true);
          if (m.get_keys_loaded() == special) {
            m.write_byte_internal(O_IO+IO_IR, m.read_byte(O_IO+IO_IR) | INT_JOY);
          }
          }
          break;
        }
        case SDLK_RETURN: {
          if (m.get_keys(special) & JOYP_START_BIT) {
            m.update_keys(special, JOYP_START_BIT, true);
          if (m.get_keys_loaded() == special) {
            m.write_byte_internal(O_IO+IO_IR, m.read_byte(O_IO+IO_IR) | INT_JOY);
          }
          }
          break;
        }
        case SDLK_BACKSPACE: {
          if (m.get_keys(special) & JOYP_SELECT_BIT) {
            m.update_keys(special, JOYP_SELECT_BIT, true);
          if (m.get_keys_loaded() == special) {
            m.write_byte_internal(O_IO+IO_IR, m.read_byte(O_IO+IO_IR) | INT_JOY);
          }
          }
          break;
        }
        case SDLK_UP: {
          if (m.get_keys(direction) & JOYP_U_BIT) {
            m.update_keys(direction, JOYP_U_BIT, true);
          if (m.get_keys_loaded() == direction) {
            m.write_byte_internal(O_IO+IO_IR, m.read_byte(O_IO+IO_IR) | INT_JOY);
          }
          }
          break;
        }
        case SDLK_DOWN: {
          if (m.get_keys(direction) & JOYP_D_BIT) {
            m.update_keys(direction, JOYP_D_BIT, true);
          if (m.get_keys_loaded() == direction) {
            m.write_byte_internal(O_IO+IO_IR, m.read_byte(O_IO+IO_IR) | INT_JOY);
          }
          }
          break;
        }
        case SDLK_LEFT: {
          if (m.get_keys(direction) & JOYP_L_BIT) {
            m.update_keys(direction, JOYP_L_BIT, true);
          if (m.get_keys_loaded() == direction) {
            m.write_byte_internal(O_IO+IO_IR, m.read_byte(O_IO+IO_IR) | INT_JOY);
          }
          }
          break;
        }
        case SDLK_RIGHT: {
          if (m.get_keys(direction) & JOYP_R_BIT) {
            m.update_keys(direction, JOYP_R_BIT, true);
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
          return 0;
        }
      }
    }
  }
  return 1;
};
