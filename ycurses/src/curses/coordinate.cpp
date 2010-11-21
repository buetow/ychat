#ifndef COORDONATE_CPP
#define COORDONATE_CPP

#include "coordinate.h"

coordinate::coordinate()
{
  y = x = 0;
}

coordinate::coordinate(coordinate_type type, dummy_window& r_win)
{
  set(type, r_win);
}

coordinate::coordinate(coordinate_type type)
{
  set(type);
}

coordinate::coordinate(int y, int x)
{
  this->y = y;
  this->x = x;
}

coordinate&
coordinate::set(coordinate_type type, dummy_window& r_win)
{
  switch (type)
  {
    case Absolutecoord:
    	getyx(r_win.get_WINDOW(), y, x);
	break;

    case Relativecoord:
    	getparyx(r_win.get_WINDOW(), y, x);
	break;

    case Beginningcoord:
       	getbegyx(r_win.get_WINDOW(), y, x);
	break;

    case windowSize:
    	getmaxyx(r_win.get_WINDOW(), y, x);
	break;

    default:
	set(type);
  }

  return *this;
}

coordinate&
coordinate::set(coordinate_type type)
{
  switch (type)
  {
    case TerminalSize:
    	y = COLS;
    	x = LINES;
	break;

    case TerminalCenter:
    	y = static_cast<int>(COLS / 2);
    	x = static_cast<int>(LINES / 2);
	break;
  }

  return *this;
}

coordinate&
coordinate::left()
{
  return left(1);
}

coordinate&
coordinate::right()
{
  return right(1);
}

coordinate&
coordinate::up()
{
  return up(1);
}

coordinate&
coordinate::down()
{
  return down(1);
}

coordinate&
coordinate::left(int i)
{
  x -= i;
  return *this;
}

coordinate&
coordinate::right(int i)
{
  x += i;
  return *this;
}

coordinate&
coordinate::up(int i)
{
  y -= i;
  return *this;
}

coordinate&
coordinate::down(int i)
{
  y += i;
  return *this;
}

coordinate&
coordinate::displace(int i_y, int i_x)
{
  y += i_y;
  x += i_x;
  return *this;
}

#endif
