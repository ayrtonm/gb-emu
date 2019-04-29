case SDLK_z: {
  m.update_keys(true, JOYP_A_BIT, 0);
  break;
}
case SDLK_x: {
  m.update_keys(true, JOYP_B_BIT, 0);
  break;
}
case SDLK_RETURN: {
  m.update_keys(true, JOYP_START_BIT, 0);
  break;
}
case SDLK_BACKSPACE: {
  m.update_keys(true, JOYP_SELECT_BIT, 0);
  break;
}
case SDLK_UP: {
  m.update_keys(false, JOYP_U_BIT, 0);
  break;
}
case SDLK_DOWN: {
  m.update_keys(false, JOYP_D_BIT, 0);
  break;
}
case SDLK_LEFT: {
  m.update_keys(false, JOYP_L_BIT, 0);
  break;
}
case SDLK_RIGHT: {
  m.update_keys(false, JOYP_R_BIT, 0);
  break;
}
