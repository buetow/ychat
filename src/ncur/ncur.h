/*:*
 *: File: ./src/ncur/ncur.h
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

#ifndef NCUR_H
#define NCUR_H

#include <ncurses.h>
#include <list>

#include "menu.h"
#include "../thrd/thro.h"

using namespace std;

class ncur : public thro
{
private:
  friend class sign;
  menu*   p_menu;
  WINDOW* p_serveroutput;
  list<char*>* p_messagelist;   // contains the messages for p_serveroutput!
  int  i_message_length;        // the maximum length of a system message!
  bool b_is_ready;	       	  // is set to TRUE if the admin interface is initialized.
  static void init_ncurses();
  static void close_ncurses();

  pthread_mutex_t mut_messages;
  pthread_mutex_t mut_is_ready;

public:
  ncur();
  ~ncur();

  void start( void *p_void );
  void print( char* c_print );
  void print( string  s_msg );
  void print( string* p_msg );
  void is_ready( bool b_is_ready );
  bool is_ready();
  static void switch_main_menu_( int i_choice );
  void shutdown();

  void activate_menu_win()
  {
    p_menu->activate_menu_win();
  }
};

#endif
#endif
