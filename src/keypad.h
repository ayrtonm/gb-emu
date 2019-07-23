#ifndef KEYPAD_H
#define KEYPAD_H
#include <SDL2/SDL.h>
#include <unordered_map>
#include <vector>
#include "mem.h"

using namespace std;

enum request {none, resize, quit, boost, savestate, loadstate};
enum key {Up, Down, Left, Right, A, B, Start, Select, Quit, Boost, Save, Load};

class keypad {
  public:
    keypad(string configfile);
    request handle_events(int dt, mem &m);
  private:
    SDL_Event event;
    int clk;
    string keyoption = "key";
    vector<key> keyset;
    vector<key> required_keys = {Up, Down, Left, Right, A, B, Start, Select, Quit, Boost, Save, Load};
    unordered_map<SDL_Keycode, key> keymap;
    //the index and value are the opposite of keymap since we iterate through the possible values of key to find keys that weren't set in the config file
    unordered_map<key, SDL_Keycode> default_keymap = {
      {Up, SDLK_UP},
      {Down, SDLK_DOWN},
      {Left, SDLK_LEFT},
      {Right, SDLK_RIGHT},
      {A, SDLK_z},
      {B, SDLK_x},
      {Start, SDLK_RETURN},
      {Select, SDLK_BACKSPACE},
      {Quit, SDLK_q},
      {Boost, SDLK_SPACE},
      {Save, SDLK_p},
      {Load, SDLK_o}};
    unordered_map<string, SDL_Keycode> value_to_keycode = {
      {"a", SDLK_a},
      {"b", SDLK_b},
      {"c", SDLK_c},
      {"d", SDLK_d},
      {"e", SDLK_e},
      {"f", SDLK_f},
      {"g", SDLK_g},
      {"h", SDLK_h},
      {"i", SDLK_i},
      {"j", SDLK_j},
      {"k", SDLK_k},
      {"l", SDLK_l},
      {"m", SDLK_m},
      {"n", SDLK_n},
      {"o", SDLK_o},
      {"p", SDLK_p},
      {"q", SDLK_q},
      {"r", SDLK_r},
      {"s", SDLK_s},
      {"t", SDLK_t},
      {"u", SDLK_u},
      {"v", SDLK_v},
      {"w", SDLK_w},
      {"x", SDLK_x},
      {"y", SDLK_y},
      {"z", SDLK_z},
      {"up", SDLK_UP},
      {"down", SDLK_DOWN},
      {"left", SDLK_LEFT},
      {"right", SDLK_RIGHT},
      {"enter", SDLK_RETURN},
      {"return", SDLK_RETURN},
      {"backspace", SDLK_BACKSPACE}};
    unordered_map<string, key> option_to_key = {
      {"key.a", A},
      {"key.b", B},
      {"key.start", Start},
      {"key.select", Select},
      {"key.up", Up},
      {"key.down", Down},
      {"key.left", Left},
      {"key.right", Right}};
};

#endif
