#include "../incl.h"

#ifndef STATS_H
#define STATS_H

#include "../tool/tool.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>

using namespace std;

class stats
{
private:
  // Specifies the max. amount of elements in vec_rusage_history;
  int i_rusage_vec_size;
  // History of the last i_rusage_vec_size rusage values.
  vector< map<string,long> > vec_rusage_history;
  pthread_mutex_t mut_vec_rusage;

  int i_num_rooms; //<<
  pthread_mutex_t mut_num_rooms; //<<

  void set_rusage_vec_size( int i_rusage_vec_size );

public:
  stats( );
  ~stats( );

  //<<*
  int get_num_rooms();
  void increment_num_rooms();
  void decrement_num_rooms();
  //*>>
  void update_rusage_history();
  string get_rusage_history( string s_type, string s_seperator );
  long get_ru_maxrss();
  //<<*
#ifdef NCURSES

  void print_num_rooms();
#endif
  //*>>
};

#endif
