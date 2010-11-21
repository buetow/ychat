
#ifndef COLOR_H
#define COLOR_H

#include "incl.h"

using namespace std;

const short Red = COLOR_RED;
const short Green = COLOR_GREEN;
const short Yellow = COLOR_YELLOW;
const short Blue = COLOR_BLUE;
const short Cyan = COLOR_CYAN;
const short Magenta = COLOR_MAGENTA;
const short White = COLOR_WHITE;
const short Black = COLOR_BLACK;

class color 
{
 private:
  void init();  

  static int i_pair_count;
  short i_pair;
  short i_foreground;
  short i_background;
  static bool b_activated;

  friend class curses;

 public:
  color(short i, short j);
  ~color();
  void enable();
  void disable();
  short get_num() { return i_pair; }
};

#endif
