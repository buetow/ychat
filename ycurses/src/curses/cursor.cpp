#ifndef CURSOR_CPP
#define CURSOR_CPP

#include "cursor.h"

cursor::cursor()
{
}

cursor::cursor(cursorType t)
{
  set(t);
}

cursor::cursor(coordinate& r_coord)
{
  set(r_coord);
}

void
cursor::set(cursorType t)
{
  curs_set(t);
}

void
cursor::set(coordinate& r_coord)
{
  move(r_coord.y, r_coord.x);
}

#endif
