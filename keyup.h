case SDLK_z: {
  if (~m.get_keys(true) & JOYP_A_BIT) {
    m.update_keys(true, JOYP_A_BIT, false);
  }
  break;
}
case SDLK_x: {
  if (~m.get_keys(true) & JOYP_B_BIT) {
    m.update_keys(true, JOYP_B_BIT, false);
  }
  break;
}
case SDLK_RETURN: {
  if (~m.get_keys(true) & JOYP_START_BIT) {
    m.update_keys(true, JOYP_START_BIT, false);
  }
  break;
}
case SDLK_BACKSPACE: {
  if (~m.get_keys(true) & JOYP_SELECT_BIT) {
    m.update_keys(true, JOYP_SELECT_BIT, false);
  }
  break;
}
case SDLK_UP: {
  if (~m.get_keys(false) & JOYP_U_BIT) {
    m.update_keys(false, JOYP_U_BIT, false);
  }
  break;
}
case SDLK_DOWN: {
  if (~m.get_keys(false) & JOYP_D_BIT) {
    m.update_keys(false, JOYP_D_BIT, false);
  }
  break;
}
case SDLK_LEFT: {
  if (~m.get_keys(false) & JOYP_L_BIT) {
    m.update_keys(false, JOYP_L_BIT, false);
  }
  break;
}
case SDLK_RIGHT: {
  if (~m.get_keys(false) & JOYP_R_BIT) {
    m.update_keys(false, JOYP_R_BIT, false);
  }
  break;
}
