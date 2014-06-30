/*:*
 *: File: ./src/ncur/ncur.cpp
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

#ifndef NCUR_CPP
#define NCUR_CPP

#include "ncur.h"

#ifdef CLI
#include "../cli/cli.h"
#endif
#include "../sign.h"

using namespace std;

#ifdef NCURSES

const string GMAKE_PARAMS[] =
  { "clean_base", "clean_modules",  "all"
  };
const int    GMAKE_ELEMENTS = 3;

ncur::ncur( )
{
  p_messagelist = new list<char*>;
  pthread_mutex_init( &mut_messages, NULL );
  pthread_mutex_init( &mut_is_ready, NULL );
  i_message_length = 45;
  b_is_ready = false;
}

ncur::~ncur()
{
  pthread_mutex_destroy( &mut_messages );
  pthread_mutex_destroy( &mut_is_ready );
}

void
ncur::start( void *p_void )
{
  ncur::init_ncurses();

  char *choices[] = {
                      "Unload all modules      ", //<<
                      "Reload all modules      ", //<<
                      //>>"                        ",
                      //>>"                        ",
                      "Clear template cache    ",
                      "Run garbage collector   ", //<<
                      //>>"                        ",
                      "Show max res. set size  ",
                      "Compile changed sources ",
                      "Recompile all sources   ",
                      "Show source stats       ",
                      "Command line interface  ",
                      //<<*
#ifdef DATABASE
                      "Close DB connections    ",
#else
                      "			   ",
#endif
                      //*>>
                      //>>"                        ",
                      "Shut down server"
                    };

  p_serveroutput = newwin( 19, 49, 1, 31 );
  wbkgd(p_serveroutput, COLOR_PAIR(1));

  box      ( p_serveroutput, 0, 0 );
  mvwprintw( p_serveroutput, 2, 2, NCURMSG );
  wrefresh ( p_serveroutput );

  print( string("yChat ") + VERSION );


  p_menu = new menu( 1, 1, 30, 19, NCURADM, choices, 11, COLOR_PAIR(1));

  mvprintw(NCUR_SERVER_HEADER_X,NCUR_SERVER_HEADER_Y, NCURSE0);
  mvprintw(NCUR_POOL_HEADER_X,NCUR_POOL_HEADER_Y, NCURSE1);
  mvprintw(NCUR_DATA_HEADER_X,NCUR_DATA_HEADER_Y, NCURSE2); //<<
  mvprintw(NCUR_CHAT_HEADER_X,NCUR_CHAT_HEADER_Y, NCURSE3); //<<
  mvprintw(NCUR_CACHED_HEADER_X,NCUR_CACHED_HEADER_Y, NCURSE4);

  wrap::HTML->print_cached(0);

  is_ready(true);
  wrap::SMAN->print_init_ncurses(); //<<
  wrap::STAT->print_num_rooms();    //<<

  p_menu->start( &switch_main_menu_ );

  shutdown();
}

void
ncur::shutdown()
{
  ncur::close_ncurses();
}


void
ncur::print( string *p_msg )
{
  print( *p_msg );
}

void
ncur::print( string s_msg )
{
  print( (char*)s_msg.c_str() );
}

void
ncur::print( char* c_print )
{
  // Removing \n
  if ( strlen(c_print) > i_message_length )
  {
    string s_tmp(c_print);
    print( s_tmp.substr( 0, i_message_length ) );
    print( s_tmp.substr( i_message_length, s_tmp.length()-i_message_length ) );
    return;
  }

  int i;
  char* c_temp = new char[i_message_length];
  memcpy( c_temp, c_print, strlen(c_print) );

  for ( i = strlen(c_print); i < i_message_length; ++i )
    c_temp[i] = ' ';

  c_temp[i] = '\0';

  pthread_mutex_lock( &mut_messages );

  if ( p_messagelist->size() > 12 )
  {
    char* c_front = p_messagelist->front();
    p_messagelist->pop_front();
    free(c_front);
  }

  p_messagelist->push_back( c_temp );


  if ( is_ready() )
  {
    list<char*>::iterator iter;
    iter = p_messagelist->begin();

    for ( i = 4; i < 18 && iter != p_messagelist->end(); ++i, ++iter )
      mvwprintw( p_serveroutput, i, 2, *iter );

    wrefresh ( p_serveroutput );
  }

  pthread_mutex_unlock( &mut_messages );
}

void
ncur::switch_main_menu_( int i_choice )
{
  int i;

  if( i_choice != 0 )
    switch ( i_choice )
    {
      //<<*
    case 1:
      wrap::MODL->unload_modules();
      mvprintw( 20,2, "Unloaded all modules                             ");
      refresh();
      break;
    case 2:
      wrap::MODL->reload_modules();
      mvprintw( 20,2, "Reloaded all modules                            ");
      refresh();
      break;
      //*>>
    case 3:
      wrap::HTML->clear_cache();
      mvprintw( 20,2, "Cleared the template cache                                   ");
      refresh();
      break;
    case 4:
      //<<*
      if ( ! wrap::GCOL->remove_garbage() )
        wrap::NCUR->print( GAROFFNE );
      mvprintw( 20,2, "Garbage collector activated                                  ");
      //*>>
      refresh();
      break;
    case 5:
      mvprintw( 20,2, "Showing max resident set size in memory                      ");
      wrap::NCUR->print( STATRSS + string("(") + tool::int2string(
                           wrap::STAT->get_ru_maxrss()) + string(")"));
      break;
    case 6:
      tool::shell_command( string(GMAKE), METH_NCURSES);
      break;
    case 7:
      for ( i = 0; i < GMAKE_ELEMENTS; i++ )
        tool::shell_command( GMAKE + GMAKE_PARAMS[i], METH_NCURSES);
      break;
    case 8:
      tool::shell_command( string(GMAKE) + " stats", METH_NCURSES);
      break;
    case 9:
#ifdef CLI

      wrap::NCUR->is_ready(false);
      refresh();			/* Print it on to the real screen */

      def_prog_mode();		/* Save the tty modes		  */
      endwin();			/* End curses mode temporarily	  */
      delete new cli();		        /* Start CLI mode		  */
      reset_prog_mode();		/* Return to the previous tty mode*/
      /* stored by def_prog_mode() 	  */
      refresh();			/* Do refresh() to restore the	  */
      /* Screen contents		  */
      wrap::NCUR->is_ready(true);
      wrap::NCUR->activate_menu_win();
#else

      mvprintw( 20,2, "CLI mode has not been compiled in!                           ");
#endif

      break;
    case 10:
#ifdef DATABASE

      wrap::DATA->disconnect_all_connections(); 	//<<
#endif

      break;

    case 11: // Shut down server
      sign::terminate_received(0);
      break;

    default:
      mvprintw( 20,2, "Selection # %d not yet implemented!", i_choice-1);
      wrap::NCUR->print( "Selection not yet implemented!" );
      refresh();
      break;
    }
}

void
ncur::init_ncurses()
{
  initscr();
  start_color();
  clear();
  noecho();
  cbreak();       // Line buffering disabled. pass on everything
  init_pair(1, COLOR_BLACK, COLOR_CYAN);
  mvprintw( 0,2, (char*)(tool::ychat_version()).c_str());
  curs_set(0);
  refresh();
}

void
ncur::close_ncurses()
{
  refresh();
  clrtoeol();
  refresh();
  endwin();
}

void
ncur::is_ready( bool b_is_ready )
{
  pthread_mutex_lock( &mut_is_ready );
  this->b_is_ready = b_is_ready;
  pthread_mutex_unlock( &mut_is_ready );
}

bool
ncur::is_ready()
{
  bool b_ret;
  pthread_mutex_lock( &mut_is_ready );
  b_ret = b_is_ready;
  pthread_mutex_unlock( &mut_is_ready );
  return b_ret;
}

#endif
#endif
