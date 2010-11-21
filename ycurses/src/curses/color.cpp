#ifndef COLOR_CPP
#define COLOR_CPP

#include "color.h"

bool color::b_activated = false;
int color::i_pair_count = 0;

color::color(short i, short j) 
	: i_foreground(i), i_background(j)
{
  init();

  i_pair = ++i_pair_count;

  /*
   * Simple color assignment.  color pair 0 cannot
   * be redefined.
   */  
  init_pair(i_pair, i_foreground, i_background);
}

color::~color()
{
}

void
color::init()
{
  if ( !b_activated )
  {
    if (has_colors())
    {
      start_color();
      b_activated = true;
    }
  }
}

void
color::enable()
{
  attron(COLOR_PAIR(i_pair));
}

void
color::disable()
{
  attroff(COLOR_PAIR(i_pair));
}


#endif

