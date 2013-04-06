#include "../incl.h"

#ifndef TIMR_H
#define TIMR_H

#include "../thrd/thro.h"

#include <unistd.h>

using namespace std;

class timr : public thro
{
private:
  bool b_timer_active;
  int  i_time_offset;
  string s_uptime;
  string s_time;

  pthread_mutex_t mut_s_time;
  pthread_mutex_t mut_s_uptime;
  pthread_mutex_t mut_i_offset;

public:
  timr();
  ~timr();

  bool get_timer_active() const;
  void start( void *v_ptr );

#ifdef NCURSES

  void print_time();
#endif

  void set_time( double d_uptime, int i_cur_seconds, int i_cur_minutes, int i_cur_hours );
  string add_zero_to_front( string s_time );

  // inline for dynamic module access!
  string
  get_time(  )
  {
    string s_ret;
    pthread_mutex_lock  ( &mut_s_time );
    s_ret = this->s_time;
    pthread_mutex_unlock( &mut_s_time );
    return s_ret;
  }

  string
  get_uptime(  )
  {
    string s_ret;
    pthread_mutex_lock  ( &mut_s_uptime );
    s_ret = this->s_uptime;
    pthread_mutex_unlock( &mut_s_uptime );
    return s_ret;
  }

  int get_offset();
  double get_time_diff( time_t &clock_diff );
};

#endif
