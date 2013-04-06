/*
 * yChat++; Homepage: www.yChat.org
 * Copyright (C) 2003 Paul C. Buetow ( Snooper@yChat.org, ICQ: 11655527 )
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


// needed for ignoring SIGPIPE.
#include <signal.h>

// include header files which are included from every class too.
#include "incl.h"

// include the chat manager.
#include "CHAT.h"

// include the config manager.
#include "CONF.h"

// include the html-template manager.
#include "HTML.h"

// include the mutex manager for global synchronization.
#include "MUTX.h"

// include the socket manager.
#include "SOCK.h"

using namespace std;

int main()
{
#ifdef _VERBOSE

cout  <<  "         ___ _           _   "     << endl
      <<  " _   _  / __\\ |__   __ _| |_ "    << endl
      <<  "| | | |/ /  | '_ \\ / _` | __|"    << endl
      <<  "| |_| / /___| | | | (_| | |_ "     << endl
      <<  " \\__, \\____/|_| |_|\\__,_|\\__|" << endl
      <<  " |___/ 			"     << endl << endl 

      << DESCRIP  
      << VERSION << endl
      << VERSION << endl
      << CONTACT << endl 
      << SEPERAT << endl 
      << STARTMS << endl ;
#endif

 // ignore SIGPIPE. otherwise the server will shut down with "Broken pipe" if
 // a client unexpected disconnects himself from a SOCK_STREAM.
 signal( SIGPIPE, SIG_IGN );

 // all the static data classes have to be initialized once. otherwise they will
 // contain only empty pointers and the chat server won't work correctly. 
 // the order of the initializations is very importand. for example the HTML::init()
 // invokations assumes an initialized CONF class.
 MUTX::init(); // init the mutex manager.
 CONF::init(); // init the config manager.
 HTML::init(); // init the html-template manager. 
 SOCK::init(); // init the socket manager. 
 CHAT::init(); // init the chat manager. 

 // start the socket manager. this one will listen for incoming http requests and will
 // forward them to the specified routines which will generate a http response.
 SOCK::get().start();

#ifdef _VERBOSE
 cout << DOWNMSG << endl;
#endif

 return 0;
}
