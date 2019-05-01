case SDLK_z: {
  if (m.get_keys(true) & JOYP_A_BIT) {
    m.update_keys(true, JOYP_A_BIT, true);
  if (!m.direction_loaded()) {
    m.write_byte(O_IO+IO_IR, m.read_byte(O_IO+IO_IR) | INT_JOY);
  }
  }
  break;
}
case SDLK_x: {
  if (m.get_keys(true) & JOYP_B_BIT) {
    m.update_keys(true, JOYP_B_BIT, true);
  if (!m.direction_loaded()) {
    m.write_byte(O_IO+IO_IR, m.read_byte(O_IO+IO_IR) | INT_JOY);
  }
  }
  break;
}
case SDLK_RETURN: {
  if (m.get_keys(true) & JOYP_START_BIT) {
    m.update_keys(true, JOYP_START_BIT, true);
  if (!m.direction_loaded()) {
    m.write_byte(O_IO+IO_IR, m.read_byte(O_IO+IO_IR) | INT_JOY);
  }
  }
  break;
}
case SDLK_BACKSPACE: {
  if (m.get_keys(true) & JOYP_SELECT_BIT) {
    m.update_keys(true, JOYP_SELECT_BIT, true);
  if (!m.direction_loaded()) {
    m.write_byte(O_IO+IO_IR, m.read_byte(O_IO+IO_IR) | INT_JOY);
  }
  }
  break;
}
case SDLK_UP: {
  if (m.get_keys(false) & JOYP_U_BIT) {
    m.update_keys(false, JOYP_U_BIT, true);
  if (m.direction_loaded()) {
    m.write_byte(O_IO+IO_IR, m.read_byte(O_IO+IO_IR) | INT_JOY);
  }
  }
  break;
}
case SDLK_DOWN: {
  if (m.get_keys(false) & JOYP_D_BIT) {
    m.update_keys(false, JOYP_D_BIT, true);
  if (m.direction_loaded()) {
    m.write_byte(O_IO+IO_IR, m.read_byte(O_IO+IO_IR) | INT_JOY);
  }
  }
  break;
}
case SDLK_LEFT: {
  if (m.get_keys(false) & JOYP_L_BIT) {
    m.update_keys(false, JOYP_L_BIT, true);
  if (m.direction_loaded()) {
    m.write_byte(O_IO+IO_IR, m.read_byte(O_IO+IO_IR) | INT_JOY);
  }
  }
  break;
}
case SDLK_RIGHT: {
  if (m.get_keys(false) & JOYP_R_BIT) {
    m.update_keys(false, JOYP_R_BIT, true);
  if (m.direction_loaded()) {
    m.write_byte(O_IO+IO_IR, m.read_byte(O_IO+IO_IR) | INT_JOY);
  }
  }
  break;
}
case SDLK_q: {
  if (m.check_memory_dump()) {
    m.dump_memory();
  }
  return 0;
}
