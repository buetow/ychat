#include "../incl.h"

#ifndef TIMO_H
#define TIMO_H

using namespace std;

class timo // timeout class
{
protected:
  time_t t_time;   // last activity time.
  pthread_mutex_t mut_t_time;

public:
  timo( );
  ~timo( );

  double get_last_activity();
  void renew_timeout();
};

#endif
