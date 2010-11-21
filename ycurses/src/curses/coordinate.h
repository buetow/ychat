#ifndef COORDINATE_H
#define COORDINATE_H

#include "incl.h"
#include "dummy.h"

using namespace std;

enum coordinate_type
{
  Absolutecoord,
  Relativecoord,
  Beginningcoord,
  windowSize,
  TerminalSize,
  TerminalCenter
};


struct coordinate
{
 int y, x;

 coordinate();
 coordinate(coordinate_type type, dummy_window& r_win);
 coordinate(coordinate_type type);
 coordinate(int y, int x);

 coordinate& set(coordinate_type type, dummy_window& r_win);
 coordinate& set(coordinate_type type);
 coordinate& left();
 coordinate& right();
 coordinate& up();
 coordinate& down();
 coordinate& left(int i);
 coordinate& right(int i);
 coordinate& up(int i);
 coordinate& down(int i);
 coordinate& displace(int i_y, int i_x);

};

#endif
