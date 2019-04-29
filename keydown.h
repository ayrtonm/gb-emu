case SDLK_q: {
  if (m.check_memory_dump()) {
    m.dump_memory();
  }
  return 0;
}
case SDLK_z: {
  m.update_keys(true, JOYP_A_BIT, 1);
  m.write_byte(O_IO+IO_IR, m.read_byte(O_IO+IO_IR) | INT_JOY);
  break;
}
case SDLK_x: {
  m.update_keys(true, JOYP_B_BIT, 1);
  m.write_byte(O_IO+IO_IR, m.read_byte(O_IO+IO_IR) | INT_JOY);
  break;
}
case SDLK_RETURN: {
  m.update_keys(true, JOYP_START_BIT, 1);
  m.write_byte(O_IO+IO_IR, m.read_byte(O_IO+IO_IR) | INT_JOY);
  break;
}
case SDLK_BACKSPACE: {
  m.update_keys(true, JOYP_SELECT_BIT, 1);
  m.write_byte(O_IO+IO_IR, m.read_byte(O_IO+IO_IR) | INT_JOY);
  break;
}
case SDLK_UP: {
  m.update_keys(false, JOYP_U_BIT, 1);
  m.write_byte(O_IO+IO_IR, m.read_byte(O_IO+IO_IR) | INT_JOY);
  break;
}
case SDLK_DOWN: {
  m.update_keys(false, JOYP_D_BIT, 1);
  m.write_byte(O_IO+IO_IR, m.read_byte(O_IO+IO_IR) | INT_JOY);
  break;
}
case SDLK_LEFT: {
  m.update_keys(false, JOYP_L_BIT, 1);
  m.write_byte(O_IO+IO_IR, m.read_byte(O_IO+IO_IR) | INT_JOY);
  break;
}
case SDLK_RIGHT: {
  m.update_keys(false, JOYP_R_BIT, 1);
  m.write_byte(O_IO+IO_IR, m.read_byte(O_IO+IO_IR) | INT_JOY);
  break;
}
