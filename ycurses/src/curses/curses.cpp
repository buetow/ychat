#ifndef CURSES_CPP
#define CURSES_CPP

#include "curses.h"

int curses::i_self_counter = 0;

curses::curses()
{
  if ( ++i_self_counter == 1 )
  {
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
  }
}

curses::~curses()
{
  if ( --i_self_counter == 0 )
  {
/*
    Factory<attributes>::destroyAll();
    Factory<color>::destroyAll();
    Factory<window>::destroyAll();
    Factory<menu>::destroyAll();
    Factory<pwindow>::destroyAll();
 */ 
    clear();
    endwin();
  }
}

void
curses::finish()
{
}

int
curses::get_char()
{
  return getch();
}

string
curses::get_string()
{
  char c[1024];
  getstr(c); 
  return string(c);
}

void
curses::pause()
{
  getch();
}

void
curses::disable()
{
  refresh();
  def_prog_mode();
  endwin();
}

void
curses::enable()
{
  reset_prog_mode();
  refresh();
}

void
curses::clear()
{
  coordinate coord(LINES, COLS); 
  for ( int i = 0; i < coord.y; ++i )
    for ( int j = 0; j < coord.x; ++j )
      mvaddch(i, j, ' ');

  refresh();
}

#endif
