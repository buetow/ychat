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
  ncur( );      	      // a standard constructor.
  ~ncur( );

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
