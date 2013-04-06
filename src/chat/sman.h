#include "../incl.h"

#ifndef SMAN_H
#define SMAN_H

#include <cstdlib>

#include "sess.h"
#include "../maps/shashmap.h"
#include "../monitor/dump.h"

using namespace std;

class sman : public shashmap<sess*>
{
private:
  string generate_id( int i_len );
  int i_session_count;

  pthread_mutex_t mut_i_session_count;
#ifdef NCURSES

  void print_sessions();
#endif

public:
  sman();
  ~sman();
  sess *get_session( string s_tmpid );
  int get_session_count( );
  sess *create_session( );
  void destroy_session( string s_tmpid );
#ifdef NCURSES

  void print_init_ncurses();
#endif

};


#endif

