/*
 * yhttpd++; Contact: www.yhttpd.org; Mail@yhttpd.org
 * Copyright (C) 2003 Paul C. Buetow, Volker Richter 
 * Copyright (C) 2004 Paul C. Buetow
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

using namespace std;

map<string,string>*
parse_argc( int argc, char* argv[] )
{
 map<string,string>* start_params = new map<string,string>;

 string s_output = "";

 // Set to 1 if a config option key has to be read ( ./yhttpd -o key1 value1 -o key2 value2 ... );
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
    s_output.append(tool::yhttpd_version()+"\n");
   if ( string(argv[i]).find("h") != string::npos )
    s_output.append( "Usage: ./yhttpd {h|v}|{o confkey confvalue}\n" );
   if ( string(argv[i]).find("o") != string::npos )
    b_conf = 1;
  }
 }

 if ( s_output.compare("") != 0 )
 {
  cout << s_output;
  delete start_params;
  exit(0);
 }

 return start_params;
}

int
main(int argc, char* argv[])
{
 map<string,string>* p_start_params = parse_argc( argc, argv );

    cout << tool::yhttpd_version() << endl 
         << DESCRIP << endl
    	 << DESCRI2 << endl
         << CONTACT << endl
         << SEPERAT << endl;

    // ignore SIGPIPE. otherwise the server will shut down with "Broken pipe" if
    // a client unexpected disconnects himself from a SOCK_STREAM.
    signal( SIGPIPE, SIG_IGN );

    // all the static data classes have to be initialized once. otherwise they will
    // contain only empty pointers and the chat server won't work correctly.
    // the order of the initializations is very importand. for example the s_html::init()
    // invokations assumes an initialized s_conf class.
    // begin to draw the ncurses amdin interface in a new pthread.

    // init the dynamic wrapper (is needed to pass all wrapped objects through a single pointer).
    wrap::WRAP = new dynamic_wrap;

    // init the config manager.
    wrap::WRAP->CONF = wrap::CONF = new conf( CONFILE, p_start_params ); 
    delete p_start_params,


   // init the statistic manager.
    wrap::WRAP->STAT = wrap::STAT = new stats; 

    // init the html-template manager.
    wrap::WRAP->HTML = wrap::HTML = new html; 

    // init the system message logd 
    wrap::WRAP->LOGD = wrap::LOGD = new logd( wrap::CONF->get_elem("httpd.logging.systemfile"),
                                              wrap::CONF->get_elem("httpd.logging.systemlines") );
    

    // init the socket manager.
    wrap::WRAP->SOCK = wrap::SOCK = new sock; 

#ifdef NCURSES
    wrap::WRAP->NCUR = wrap::NCUR = new ncur; 	// init the ncurses admin interface.
    wrap::NCUR->run();				// run the thread

    // wait until ncurses interface has been initialized.
    while ( ! wrap::NCUR->is_ready() )
     usleep(1000);
#endif


    // init the system timer.
    wrap::WRAP->TIMR = wrap::TIMR = new timr; 
    wrap::TIMR->run(); // run the thread

    // init the module-loader manager.
    wrap::WRAP->MODL = wrap::MODL = new modl; 


#ifndef NCURSES
#ifdef CLI
    cli* p_cli = new cli;
    p_cli->run();
#endif
#endif


    // start the socket manager. this one will listen for incoming http requests and will
    // forward them to the specified routines which will generate a http response.
    wrap::SOCK->start();

    cout << DOWNMSG << endl;

    return 0;
}
