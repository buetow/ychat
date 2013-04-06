/*:*
 *: File: ./src/ncur/menu.h
 *: 
 *: yChat; Homepage: www.yChat.org; Version 0.7.9.5-RELEASE
 *: 
 *: Copyright (C) 2003 Paul C. Buetow, Volker Richter
 *: Copyright (C) 2004 Paul C. Buetow
 *: Copyright (C) 2005 EXA Digital Solutions GbR
 *: 
 *: This program is free software; you can redistribute it and/or
 *: modify it under the terms of the GNU General Public License
 *: as published by the Free Software Foundation; either version 2
 *: of the License, or (at your option) any later version.
 *: 
 *: This program is distributed in the hope that it will be useful,
 *: but WITHOUT ANY WARRANTY; without even the implied warranty of
 *: MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *: GNU General Public License for more details.
 *: 
 *: You should have received a copy of the GNU General Public License
 *: along with this program; if not, write to the Free Software
 *: Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *:*/

#include "../incl.h"

#ifdef NCURSES

#ifndef MENU_H
#define MENU_H

#include <ncurses.h>

using namespace std;

class menu
{
private:
  char **choices;
  char *c_header;

  int i_startx, i_starty, i_width, i_height, i_highlight, i_choice,
  i_numchoices, c;

  WINDOW *win;

  void initialize( const chtype ch );

public:
  explicit menu( int i_startx, int i_starty, int i_width, int i_height, char *c_header, char **choices, int i_numchoices, const chtype ch );
  ~menu( );

  void display();
  void start( void (*swich_case_menu_action)(int) );

  void activate_menu_win()
  {
    keypad(win, 1);
  }
};

#endif
#endif
