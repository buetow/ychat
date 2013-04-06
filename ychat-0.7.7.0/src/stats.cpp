#ifndef STATS_CPP
#define STATS_CPP

#include "stats.h"
#include "tool/tool.h"

using namespace std;

stats::stats()
{
  i_rusage_vec_size = tool::string2int(
                        wrap::CONF->get_elem("httpd.stats.rusagehistory"));

  i_num_rooms = 0; //<<

  pthread_mutex_init( &mut_vec_rusage, NULL );
  pthread_mutex_init( &mut_num_rooms, NULL ); //<<

}

stats::~stats()
{
  pthread_mutex_destroy( &mut_vec_rusage );
  pthread_mutex_destroy( &mut_num_rooms ); //<<
}

void
stats::update_rusage_history()
{
  wrap::system_message(STATUPR);

  rusage* p_rusage = new rusage;
  getrusage( RUSAGE_SELF, p_rusage );

  map<string,long> map_rusage;

  map_rusage["ru_maxrss"] = p_rusage->ru_maxrss;
  map_rusage["ru_ixrss"] = p_rusage->ru_ixrss;
  map_rusage["ru_idrss"] = p_rusage->ru_idrss;
  map_rusage["ru_isrss"] = p_rusage->ru_isrss;
  map_rusage["ru_minflt"] = p_rusage->ru_minflt;
  map_rusage["ru_majflt"] = p_rusage->ru_majflt;
  map_rusage["ru_nswap"] = p_rusage->ru_nswap;
  map_rusage["ru_inblock"] = p_rusage->ru_inblock;
  map_rusage["ru_oublock"] = p_rusage->ru_oublock;
  map_rusage["ru_msgsnd"] = p_rusage->ru_msgsnd;
  map_rusage["ru_msgrcv"] = p_rusage->ru_msgrcv;
  map_rusage["ru_nsignals"] = p_rusage->ru_nsignals;
  map_rusage["ru_nvcsw"] = p_rusage->ru_nvcsw;
  map_rusage["ru_nivcsw"] = p_rusage->ru_nivcsw;

  delete p_rusage;

  pthread_mutex_lock ( &mut_vec_rusage );

  if ( vec_rusage_history.size() >= i_rusage_vec_size )
    vec_rusage_history.erase( vec_rusage_history.begin() );

  vec_rusage_history.push_back(map_rusage);

  pthread_mutex_unlock( &mut_vec_rusage );
}

void
stats::set_rusage_vec_size( int i_rusage_vec_size )
{
  pthread_mutex_lock ( &mut_vec_rusage );
  this->i_rusage_vec_size = i_rusage_vec_size;
  pthread_mutex_unlock( &mut_vec_rusage );
}

long
stats::get_ru_maxrss()
{
  rusage* p_rusage = new rusage;
  getrusage( RUSAGE_SELF, p_rusage );

  long l_ret = p_rusage->ru_maxrss;
  delete p_rusage;

  return l_ret;
}

string
stats::get_rusage_history( string s_type, string s_seperator )
{
  string s_ret;
  int i_count = 0;
  vector< map<string,long> >::iterator iter;

  pthread_mutex_lock  ( &mut_vec_rusage );

  for ( iter = vec_rusage_history.begin();
        iter != vec_rusage_history.end();
        iter++, i_count++ )
    s_ret.append(s_seperator +
                 tool::int2string(i_count) + ". " + iter->find(s_type)->first + " " +
                 tool::int2string( iter->find(s_type)->second) + "\n");

  pthread_mutex_unlock( &mut_vec_rusage );

  return s_ret;
}

//<<*
int
stats::get_num_rooms()
{
  pthread_mutex_lock  ( &mut_num_rooms );
  int i_ret = i_num_rooms;
  pthread_mutex_unlock( &mut_num_rooms );
  return i_ret;
}

void
stats::increment_num_rooms()
{
  pthread_mutex_lock  ( &mut_num_rooms );
  ++i_num_rooms;
  pthread_mutex_unlock( &mut_num_rooms );
#ifdef NCURSES

  print_num_rooms();
#endif
}
void
stats::decrement_num_rooms()
{
  pthread_mutex_lock  ( &mut_num_rooms );
  --i_num_rooms;
  pthread_mutex_unlock( &mut_num_rooms );
#ifdef NCURSES

  print_num_rooms();
#endif
}

#ifdef NCURSES
void
stats::print_num_rooms()
{
  if ( !wrap::NCUR->is_ready() )
    return;

  mvprintw( NCUR_NUM_ROOMS_X, NCUR_NUM_ROOMS_Y, "Rooms: %d", get_num_rooms());
  refresh();
}
#endif
//*>>

#endif
