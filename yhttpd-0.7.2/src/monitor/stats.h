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


  void set_rusage_vec_size( int i_rusage_vec_size );

public:
  stats( );
  ~stats( );

  void update_rusage_history();
  string get_rusage_history( string s_type, string s_seperator );
  long get_ru_maxrss();
};

#endif
