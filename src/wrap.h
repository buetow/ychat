/*:*
 *: File: ./src/wrap.h
 *: 
 *: yChat; Homepage: www.yChat.org; Version 0.8.3-CURRENT
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

#ifndef WRAP_H
#define WRAP_H

#include "incl.h"


struct socketcontainer
{
  int i_sock;
};
#define _socket socketcontainer 
#else
#define _socket int 
#endif

#ifdef DATABASE
#include "data/data.h"
#endif
#include "chat/chat.h"
#include "conf/conf.h"
#include "chat/gcol.h"
#include "html.h"
#ifdef LOGGING
#include "logd.h"
#endif
//<<*
#include "modl.h"
//*>>

#ifdef NCURSES
#include "ncur/ncur.h"
#else
#ifdef CLI
#include "cli/cli.h"
#endif
#endif

#include "chat/sman.h"

#include "sock/sock.h"
#include "monitor/stats.h"
#include "time/timr.h"
#include "thrd/pool.h"

using namespace std;


class dynamic_wrap
{
public:
  //<<*
  chat* CHAT;
#ifdef DATABASE

  data* DATA;
#endif

  gcol* GCOL;
  sman* SMAN;
  modl* MODL;
  //*>>

  conf* CONF;
  html* HTML;
#ifdef LOGGING

  logd* LOGD;
#endif
#ifdef NCURSES

  ncur* NCUR;
#endif

  sock* SOCK;
  stats* STAT;
  timr* TIMR;
  pool* POOL;
};

class wrap
{
public:
  static void system_message( char* c_message )
  {
    wrap::system_message( string(c_message) );
  }

  static void system_message( string* p_message )
  {
    wrap::system_message( *p_message );
  }

  static void system_message( string s_message );

  static void init_wrapper(map<string,string>* p_main_loop_params);

  //<<*
  static chat* CHAT;
#ifdef DATABASE

  static data* DATA;
#endif

  static gcol* GCOL;
  static sman* SMAN;
#ifdef IRCBOT

  static ybot* YBOT;
#endif

  static modl* MODL;
  //*>>

  static conf* CONF;
  static html* HTML;
#ifdef LOGGING

  static logd* LOGD;
#endif
  static sock* SOCK;
  static stats* STAT;
  static timr* TIMR;
  static pool* POOL;
  static dynamic_wrap* WRAP;
};


#endif
