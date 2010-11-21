#ifndef CURSOR_H
#define CURSOR_H

#include "incl.h"
#include "coordinate.h"

using namespace std;

enum cursorType
{
  Invisible = 0,
  Visible,
  VeryVisible
};

struct cursor
{
  cursor(); 
  cursor(cursorType t); 
  cursor(coordinate& r_coord);

  static void set(cursorType t);
  static void set(coordinate& r_coord);

  static void set_invisible() { set(Invisible); }
  static void setVisible() { set(Visible); }
  static void setVeryVisible() { set(VeryVisible); }
};

#endif
