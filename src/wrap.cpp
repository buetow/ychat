#ifndef WRAP_CPP
#define WRAP_CPP

#include "wrap.h"

using namespace std;

//<<*
chat* wrap::CHAT;
#ifdef DATABASE
data* wrap::DATA;
#endif
gcol* wrap::GCOL;
sman* wrap::SMAN;
modl* wrap::MODL;
//*>>

conf* wrap::CONF;
html* wrap::HTML;
#ifdef LOGGING
logd* wrap::LOGD;
#endif
#ifdef NCURSES
ncur* wrap::NCUR;
#endif
sock* wrap::SOCK;
stats* wrap::STAT;
timr* wrap::TIMR;
pool* wrap::POOL;
dynamic_wrap* wrap::WRAP;

void
wrap::system_message( string s_message )
{
#ifdef NCURSES
  NCUR->print( s_message );
#endif

#ifdef SERVMSG
  cout << s_message << endl;
#endif

#ifdef LOGGING
  LOGD->log_simple_line( s_message + "\n" );
#endif
}

void
wrap::init_wrapper(map<string,string>* p_start_params)
{
  // Init the dynamic wrapper (is needed to pass all wrapped objects through a single pointer).
  WRAP = new dynamic_wrap;

  // Init the config manager.
  WRAP->CONF = CONF = new conf( CONFILE, p_start_params );
  delete p_start_params,

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


#ifdef NCURSES

  WRAP->NCUR = NCUR = new ncur; 	// init the ncurses admin interface.
  NCUR->run();				// run the thread

  // Wait until ncurses interface has been initialized.
  do {
    usleep(1000);
  } while ( ! NCUR->is_ready() );

  HTML->print_cached(0);
#else
#ifdef CLI
  cli* p_cli = new cli;
  p_cli->run();
#endif
#endif

  // Init the thread pool
  WRAP->POOL = POOL = new pool;

  // Init the socket manager.
  WRAP->SOCK = SOCK = new sock;

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
