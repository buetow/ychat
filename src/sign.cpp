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
