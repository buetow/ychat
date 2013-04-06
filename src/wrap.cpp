#ifndef WRAP_CPP
#define WRAP_CPP

#include "wrap.h"

using namespace std;


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


  // Init the system timer.
  WRAP->TIMR = TIMR = new timr;


  // Run threads
  TIMR->run();
}

#endif
