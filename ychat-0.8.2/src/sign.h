#ifndef SIGN_H
#define SIGN_H

#include "incl.h"
#include <signal.h>

class sign
{
private:
  static void clean_template_cache(int i_param);
  static void reload_dlopen_modules(int i_param); //<<
#ifdef CTCSEGV

  static void sigsev_received(int i_param);
#endif

public:
  static void init_signal_handlers();
  static void terminate_received(int i_param);
};

#endif
