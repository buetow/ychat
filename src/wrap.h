#ifndef WRAP_H
#define WRAP_H

#include "incl.h"


struct socketcontainer
{
  int i_sock;
};

#ifdef DATABASE
#endif
#include "conf/conf.h"
#include "html.h"
#ifdef LOGGING
#include "logd.h"
#endif

#ifdef NCURSES
#include "ncur/ncur.h"
#else
#ifdef CLI
#include "cli/cli.h"
#endif
#endif


#include "sock/sock.h"
#include "monitor/stats.h"
#include "time/timr.h"
#include "thrd/pool.h"


using namespace std;


class dynamic_wrap
{
public:

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


  static conf* CONF;
  static html* HTML;
#ifdef LOGGING

  static logd* LOGD;
#endif
#ifdef NCURSES

  static ncur* NCUR;
#endif

  static sock* SOCK;
  static stats* STAT;
  static timr* TIMR;
  static pool* POOL;
  static dynamic_wrap* WRAP;
};


#endif
