/*
 * yChat; Contact: www.yChat.org; Mail@yChat.org
 * Copyright (C) 2003 Paul C. Buetow, Volker Richter 
 * Copyright (C) 2004, 2005 Paul C. Buetow
 * -----------------------------------------------------------------
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 */

#include <unistd.h>
#include <signal.h>
#include "incl.h"

#ifndef NCURSES
#ifdef CLI
#include "cli/cli.h"
#endif
#endif

#include "maps/hashmap.h"

using namespace std;

map<string,string>*
parse_argc( int argc, char* argv[] )
{
  map<string,string>* start_params = new map<string,string>;

  string s_output = "";

  // Set to 1 if a config option key has to be read
  // ( ./ychat -o key1 value1 -o key2 value2 ... );
  bool b_conf = 0;

  // Will store the key of an additional option value (see also b_conf)
  string s_key;

  for (int i=1; argv[i] != 0; i++)
  {
    if ( !s_key.empty() )
    {
      (*start_params)[s_key] = string(argv[i]);
      s_key.clear();
    }
    else if ( b_conf )
    {
      s_key = string(argv[i]);
      b_conf = 0;
    }
    else
    {
      if ( string(argv[i]).find("v") != string::npos )
        s_output.append(tool::ychat_version()+"\n");

      if ( string(argv[i]).find("h") != string::npos )
        s_output.append( YCUSAGE );

      if ( string(argv[i]).find("o") != string::npos )
        b_conf = 1;
    }
  }

  if ( !s_output.empty() )
  {
    cout << s_output;
    delete start_params;
    exit(1);
  }

  return start_params;
}

int
main(int argc, char* argv[])
{
  map<string,string>* p_start_params = parse_argc( argc, argv );

  cout << tool::ychat_version() << endl
  << DESCRIP << endl
  << DESCRI2 << endl
  << CONTACT << endl
  << SEPERAT << endl;

  // Ignore SIGPIPE. otherwise the server will shut down with "Broken pipe" if
  // a client unexpected disconnects himself from a SOCK_STREAM.
  signal( SIGPIPE, SIG_IGN );

  // All the static data classes have to be initialized once. otherwise they will
  // contain only empty pointers and the chat server won't work correctly.
  // the order of the initializations is very importand. for example the s_html::init()
  // invokations assumes an initialized conf class.

  // Init the dynamic wrapper (is needed to pass all wrapped objects through a single pointer).
  wrap::WRAP = new dynamic_wrap;

  // Init the config manager.
  wrap::WRAP->CONF = wrap::CONF = new conf( CONFILE, p_start_params );
  delete p_start_params,

  // Init the statistic manager.
  wrap::WRAP->STAT = wrap::STAT = new stats;

  // Init the html-template manager.
  wrap::WRAP->HTML = wrap::HTML = new html;

#ifdef LOGGING
  // Init the system message logd
  wrap::WRAP->LOGD = wrap::LOGD = new logd( wrap::CONF->get_elem("httpd.logging.systemfile"),
                                  wrap::CONF->get_elem("httpd.logging.systemlines") );
#endif
  //<<*
  // Init the session manager.
  wrap::WRAP->SMAN = wrap::SMAN = new sman;
  //*>>


#ifdef NCURSES

  wrap::WRAP->NCUR = wrap::NCUR = new ncur; 	// init the ncurses admin interface.
  wrap::NCUR->run();				// run the thread

  // Wait until ncurses interface has been initialized.
  while ( ! wrap::NCUR->is_ready() )
    usleep(1000);

  wrap::HTML->print_cached(0);
#endif

  // Init the socket manager.
  wrap::WRAP->SOCK = wrap::SOCK = new sock;

  //<<*
  // Init the chat manager.
  wrap::WRAP->CHAT = wrap::CHAT = new chat;
  //*>>

  // Init the system timer.
  wrap::WRAP->TIMR = wrap::TIMR = new timr;
  wrap::TIMR->run(); // run the thread

  //<<*
  // Init the module-loader manager.
  wrap::WRAP->MODL = wrap::MODL = new modl;

  // Init the garbage collector
  wrap::WRAP->GCOL = wrap::GCOL = new gcol;

  // Init the data manager.
#ifdef DATABASE

  wrap::WRAP->DATA = wrap::DATA = new data;
#endif
  //*>>

#ifndef NCURSES
#ifdef CLI

  cli* p_cli = new cli;
  p_cli->run();
#endif
#endif

  //<<*
  // Initialize database connection queue
#ifdef DATABASE

  wrap::DATA->initialize_connections();
#endif
  //*>>

  // start the socket manager. this one will listen for incoming http requests and will
  // forward them to the specified routines which will generate a http response.
  wrap::SOCK->start();

  cout << DOWNMSG << endl;
  return 0;
}
