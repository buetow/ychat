/*:*
 *: File: ./src/sign.cpp
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

#ifndef SIGN_CPP
#define SIGN_CPP

#include "sign.h"

void
sign::clean_template_cache(int i_param)
{
  wrap::HTML->clear_cache();
}

//<<*
void
sign::reload_dlopen_modules(int i_param)
{
  wrap::MODL->reload_modules();
}
//*>>

#ifdef CTCSEGV
void
sign::sigsev_received(int i_param)
{
  wrap::system_message(SIGNSEG);
}
#endif

void
sign::terminate_received(int i_param)
{

#ifdef NCURSES
  //<<*
  if ( ! wrap::GCOL->remove_garbage() )
    wrap::NCUR->print( GAROFFNE );
  //*>>

  mvprintw( 21,2, "Good bye !");
  wrap::NCUR->close_ncurses();

  //<<*
#else
  wrap::GCOL->remove_garbage();
  //*>>
#endif

  exit(0);
}

void
sign::init_signal_handlers()
{
  // Ignore SIGPIPE. otherwise the server will shut down with "Broken pipe" if
  // a client unexpected disconnects himself from a SOCK_STREAM.
  signal(SIGPIPE, SIG_IGN);

  signal(SIGUSR1, clean_template_cache);
  signal(SIGUSR2, reload_dlopen_modules); //<<
#ifdef CTCSEGV
  signal(SIGSEGV, sigsev_received); 
#endif
  signal(SIGHUP, terminate_received);
  signal(SIGINT, terminate_received);
  signal(SIGTERM, terminate_received);
  //signal(SIGWINCH, );
}

#endif
