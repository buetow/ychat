/*:*
 *: File: ./src/ncur/menu.cpp
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


#include "menu.h"

#ifdef NCURSES

#ifndef MENU_CPP
#define MENU_CPP

using namespace std;

menu::menu( int i_startx, int i_starty, int i_width, int i_height, char *c_header, char **choices, int i_numchoices, const chtype ch )
{
  this->i_startx = i_startx;
  this->i_starty = i_starty;
  this->i_height = i_height;
  this->i_width  = i_width;
  this->c_header = c_header;
  this->choices  = choices;
  this->i_numchoices = i_numchoices;

  initialize( ch );
}

menu::~menu()
{
  /*
   wborder(win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
   wrefresh(win);
   delwin(win);
  */
}

void
menu::initialize( const chtype ch )
{
  this->i_highlight = 1;
  this->i_choice = 0;

  win = newwin( i_height, i_width, i_starty, i_startx );
  wbkgd(win, ch);
}

void
menu::display()
{
  int x, y, i;

  x = 2;
  y = 2;

  box( win, 0, 0 );
  mvwprintw( win, y++, x, "%s", c_header );

  for( i = 0; i < i_numchoices; i++ )
  {
    ++y;

    if( i_highlight == i+1 ) // Highlight the current selection.
    {
      wattron( win, A_REVERSE);
      mvwprintw( win, y, x, "%d. %s", i, choices[i]);
      wattroff( win, A_REVERSE);
    } else
    {
      mvwprintw( win, y, x, "%d. %s", i, choices[i]);
    }
  }

  wrefresh( win );
}

void
menu::start( void (*swich_case_menu_action)(int) )
{
  refresh();
  bool b_flag = 1;

  while( b_flag )
  {
    keypad(win, 1);
    display();
    c = wgetch( win );

    switch(c)
    {
    case KEY_UP:
      if( i_highlight == 1 )
        i_highlight = i_numchoices;
      else
        --i_highlight;
      break;

    case KEY_DOWN:
      if( i_highlight == i_numchoices )
        i_highlight = 1;
      else
        ++i_highlight;
      break;

    case 10:
      i_choice = i_highlight;
      break;

    default:
      mvprintw( NCUR_MENU_CHAR_X, NCUR_MENU_CHAR_Y, "%3d %c ", c, c);
      refresh();
      break;
    }

    // Menu action.
    ( *swich_case_menu_action ) ( i_choice );
    i_choice = 0;
  }
}

#endif
#endif
