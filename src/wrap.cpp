/*:*
 *: File: ./src/wrap.cpp
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

#ifndef WRAP_CPP
#define WRAP_CPP

#include "wrap.h"

using namespace std;

//<<*
chat* wrap::CHAT = NULL;
#ifdef DATABASE
data* wrap::DATA = NULL;
#endif
gcol* wrap::GCOL = NULL;
sman* wrap::SMAN = NULL;
modl* wrap::MODL = NULL;
//*>>

conf* wrap::CONF = NULL;
html* wrap::HTML = NULL;
#ifdef LOGGING
logd* wrap::LOGD = NULL;
#endif
#ifdef NCURSES
ncur* wrap::NCUR = NULL;
#endif
sock* wrap::SOCK = NULL;
stats* wrap::STAT = NULL;
timr* wrap::TIMR = NULL;
pool* wrap::POOL = NULL;
dynamic_wrap* wrap::WRAP = NULL;

void
wrap::system_message( string s_message )
{
#ifdef NCURSES
  if(NCUR)
  {
    NCUR->print( s_message );
  }

  else
  {
    cout << s_message << endl;
  }
#endif

#ifdef SERVMSG
  cout << s_message << endl;
#endif

#ifdef LOGGING

  LOGD->log_simple_line( s_message + "\n" );
#endif
}

void
wrap::init_wrapper(map<string,string>* p_main_loop_params)
{
  // Init the dynamic wrapper (is needed to pass all wrapped objects through a single pointer).
  WRAP = new dynamic_wrap;

  // Init the config manager.
  WRAP->CONF = CONF = new conf( CONFILE, p_main_loop_params );
  delete p_main_loop_params,

  // Init the statistic manager.
  WRAP->STAT = STAT = new stats;

  // Init the html-template manager.
  WRAP->HTML = HTML = new html;

#ifdef LOGGING
  // Init the system message logd
  WRAP->LOGD = LOGD = new logd( CONF->get_elem("httpd.logging.systemfile"),
                                CONF->get_elem("httpd.logging.systemlines") );
#endif

  //<<*
  // Init the session manager.
  WRAP->SMAN = SMAN = new sman;
  //*>>
  // Init the socket manager.
  int i_port = tool::string2int( wrap::CONF->get_elem( "httpd.serverport" ) );

  WRAP->SOCK = SOCK = new sock;

  // create the server socket and set it up to accept connections.
  if(SOCK->_make_server_socket ( i_port ) <= 0)
  {
    system_message(SOCKER1);
    exit(-1);
  }

#ifdef NCURSES

  WRAP->NCUR = NCUR = new ncur; 	// init the ncurses admin interface.
  NCUR->run();				// run the thread

  // Wait until ncurses interface has been initialized.
  do
  {
    usleep(1000);
  }
  while ( ! NCUR->is_ready() );

  HTML->print_cached(0);
#else
#ifdef CLI

  cli* p_cli = new cli;
  p_cli->run();
#endif
#endif

  // Init the thread pool
  WRAP->POOL = POOL = new pool;

  //<<*
  // Init the chat manager.
  WRAP->CHAT = CHAT = new chat;
  //*>>

  // Init the system timer.
  WRAP->TIMR = TIMR = new timr;

  //<<*
  // Init the module-loader manager.
  WRAP->MODL = MODL = new modl;

  // Init the garbage collector
  WRAP->GCOL = GCOL = new gcol;

  // Init the data manager.
#ifdef DATABASE

  WRAP->DATA = DATA = new data;
#endif
  //*>>

  // Run threads
  TIMR->run();
}

#endif
